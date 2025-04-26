#include "pch.h"
#include "Content.h"


#include <Engine/CAssetMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/assets.h>

#include <Engine/CTaskMgr.h>

#include "TreeUI.h"
#include "Inspector.h"


Content::Content()
	: m_Tree(nullptr)
	, m_NotifyHandle(nullptr)
{
}

Content::~Content()
{
}

void Content::Init()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChildUI(m_Tree);

	// Ʈ�� �ɼ� ����
	m_Tree->ShowRoot(false);
	m_Tree->ShowOnlyName(true);
	
	// Ư�� �׸��� ���õǾ����� ȣ���ų �Լ�(Delegate) ���
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&Content::Selection);

	// Ʈ�� ���� ����
	Renew();

	// Content ������ ��� ������ �޸𸮷� �ҷ��´�.
	Reload();

	m_NotifyHandle = FindFirstChangeNotification(CPathMgr::GetContentPath(), true
								, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME);
}

void Content::Update_UI()
{
	TreeNode* pSelected = m_Tree->GetSelectedNode();

	if (nullptr != pSelected)
		CAsset* pAsset = (CAsset*)pSelected->GetData();

	if (CAssetMgr::GetInst()->IsAssetChanged())
	{
		Renew();
	}

	// ������ üũ
	ObserveContent();
}

void Content::Render_UI()
{
}

void Content::Selection(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	string Name = pNode->GetName();
	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	if (nullptr == pAsset)
		return;

	Inspector* pInspector = ImGuiMgr::GetInst()->FindUI<Inspector>("Inspector");
	pInspector->SetAsset(pAsset);
}

void Content::Renew()
{
	m_Tree->Clear();

	TreeNode* pRoot = m_Tree->GetRoot();

	for (UINT i = 0; i < ASSET_TYPE::ASSET_END; ++i)
	{
		TreeNode* pAssetNode = m_Tree->AddItem(pRoot, ASSET_TYPE_STR[i], 0);
		pAssetNode->SetFrame(true);

		static vector<string> vecNames;
		vecNames.clear();
		CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)i, vecNames);

		for (size_t j = 0; j < vecNames.size(); ++j)
		{
			Ptr<CAsset> pAsset = CAssetMgr::GetInst()->FindAsset((ASSET_TYPE)i, wstring(vecNames[j].begin(), vecNames[j].end()));
			m_Tree->AddItem(pAssetNode, vecNames[j], (DWORD_PTR)pAsset.Get());
		}
	}
}



void Content::FindContentFile(const wstring& _FolderPath)
{
	wstring FolderPath = _FolderPath + L"*.*";

	WIN32_FIND_DATA FindData = {};

	// Ž�� ��� ��θ� �˷��ָ�, �װ��� Ž���ϴ� Ŀ�ο�����Ʈ ��ü�� �����ǰ�, ID(�ڵ�)�� �����Ѵ�.
	HANDLE hFind = FindFirstFile(FolderPath.c_str(), &FindData);
	assert(hFind);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	
	while (FindNextFile(hFind, &FindData))
	{
		// Ž���� ����� ������ ���
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// �̸��� .. �� ��������(���������� ������ ���)
			if (!wcscmp(FindData.cFileName, L".."))
				continue;
			
			// �������� ��Ž�� ���
			FindContentFile(_FolderPath + FindData.cFileName + L"\\");
		}

		// Ž�� ����� ������ ���
		else
		{
			wstring RelativePath = CPathMgr::GetRelativePath(_FolderPath + FindData.cFileName);
			m_ContentFile.push_back(RelativePath);
		}
	}
}


void Content::Reload()
{
	// �ε��Ǿ��ִ� ���¿� �����ϴ� ������ �����ϴ��� Ȯ��
	for (UINT i = 0; i < ASSET_END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		
		for (const auto& pair : mapAsset)
		{			
			// �������� ������ ������ ����
			if (pair.second->IsEngineAsset())
				continue;

			wstring ContentPath = CPathMgr::GetContentPath();		

			// ���������� ������ ������ �޸𸮻� �����ϴ� ���, �޸𸮿����� ������Ų��.
			if (!exists(ContentPath + pair.second->GetRelativePath()))
			{
				tTask task = { TASK_TYPE::DELETE_ASSET, (DWORD_PTR)pair.second.Get(), };
				CTaskMgr::GetInst()->AddTask(task);
			}
		}
	}

	// Content �����ȿ� ����ִ� ��� ���ϸ� �˾Ƴ���
	FindContentFile(CPathMgr::GetContentPath());

	// Content �����ȿ� �ִ� ���ϵ��� �������� �ε��ϱ�
	for (size_t i = 0; i < m_ContentFile.size(); ++i)
	{
		// Ȯ���� �̸��� ���� � ������ �������� Ȯ��
		ASSET_TYPE Type = GetAssetType(m_ContentFile[i]);

		if (ASSET_END == Type)
			continue;
		
		// Ȯ�ε� Ÿ�Կ� �°� ������ �޸𸮷� �ε�
		switch (Type)
		{
		case MESH:
			CAssetMgr::GetInst()->Load<CMesh>(m_ContentFile[i]);
			break;
		case MESHDATA:
			//CAssetMgr::GetInst()->Load<CMeshData>(m_ContentFile[i]);
			break;
		case TEXTURE:
			CAssetMgr::GetInst()->Load<CTexture>(m_ContentFile[i]);
			break;
		case MATERIAL:
			CAssetMgr::GetInst()->Load<CMaterial>(m_ContentFile[i]);
			break;
		case PREFAB:
			CAssetMgr::GetInst()->Load<CPrefab>(m_ContentFile[i]);
			break;
		case SOUND:
			CAssetMgr::GetInst()->Load<CSound>(m_ContentFile[i]);
			break;
		case SPRITE:
			CAssetMgr::GetInst()->Load<CSprite>(m_ContentFile[i]);
			break;
		case FLIPBOOK:
			CAssetMgr::GetInst()->Load<CFlipbook>(m_ContentFile[i]);
			break;
		}
	}
}

void Content::ObserveContent()
{
	// ��� Ȯ��
	DWORD dwStatus = WaitForSingleObject(m_NotifyHandle, 0);

	// ������ �̺�Ʈ�� �߻��ߴ�.
	if (dwStatus == WAIT_OBJECT_0)
	{
		Reload();
	
		FindNextChangeNotification(m_NotifyHandle);
	}
}

ASSET_TYPE Content::GetAssetType(const wstring& _RelativePath)
{
	path Ext = path(_RelativePath).extension();

	if (Ext == L".mesh" || Ext == L".MESH")
	{
		return MESH;
	}
	else if (Ext == L".mdat" || Ext == L".MDAT")
	{
		return MESHDATA;
	}
	else if ( Ext == L".png" || Ext == L".PNG"
			|| Ext == L".jpg" || Ext == L".JPG"
			|| Ext == L".jpeg" || Ext == L".JPEG"
			|| Ext == L".bmp" || Ext == L".BMP"
			|| Ext == L".tga" || Ext == L".TGA"
			|| Ext == L".dds" || Ext == L".DDS")
	{
		return TEXTURE;
	}
	else if (Ext == L".mtrl" || Ext == L".MTRL")
	{
		return MATERIAL;
	}
	else if (Ext == L".pref" || Ext == L".PREF")
	{
		return PREFAB;
	}
	else if (Ext == L".mp3" || Ext == L".MP3"
		 || Ext == L".wav" || Ext == L".WAV"
		 || Ext == L".ogg" || Ext == L".OGG")
	{
		return SOUND;		
	}
	else if (Ext == L".sprite" || Ext == L".SPRITE")
	{	
		return SPRITE;
	}
	else if (Ext == L".flip" || Ext == L".FLIP")
	{
		return FLIPBOOK;
	}

	return ASSET_TYPE::ASSET_END;
}