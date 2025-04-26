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

	// 트리 옵션 설정
	m_Tree->ShowRoot(false);
	m_Tree->ShowOnlyName(true);
	
	// 특정 항목이 선택되었을때 호출시킬 함수(Delegate) 등록
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&Content::Selection);

	// 트리 내용 갱신
	Renew();

	// Content 폴더의 모든 에셋을 메모리로 불러온다.
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

	// 변경점 체크
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

	// 탐색 대상 경로를 알려주면, 그곳을 탐색하는 커널오브젝트 객체가 생성되고, ID(핸들)을 리턴한다.
	HANDLE hFind = FindFirstFile(FolderPath.c_str(), &FindData);
	assert(hFind);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	
	while (FindNextFile(hFind, &FindData))
	{
		// 탐색한 대상이 폴더인 경우
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// 이름이 .. 인 더미폴더(상위폴더로 나가는 기능)
			if (!wcscmp(FindData.cFileName, L".."))
				continue;
			
			// 하위폴더 재탐색 재귀
			FindContentFile(_FolderPath + FindData.cFileName + L"\\");
		}

		// 탐색 대상이 파일인 경우
		else
		{
			wstring RelativePath = CPathMgr::GetRelativePath(_FolderPath + FindData.cFileName);
			m_ContentFile.push_back(RelativePath);
		}
	}
}


void Content::Reload()
{
	// 로딩되어있는 에셋에 대응하는 파일이 존재하는지 확인
	for (UINT i = 0; i < ASSET_END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		
		for (const auto& pair : mapAsset)
		{			
			// 엔진에서 제작한 에셋은 제외
			if (pair.second->IsEngineAsset())
				continue;

			wstring ContentPath = CPathMgr::GetContentPath();		

			// 원본파일이 없어진 에셋이 메모리상에 존재하는 경우, 메모리에서도 해제시킨다.
			if (!exists(ContentPath + pair.second->GetRelativePath()))
			{
				tTask task = { TASK_TYPE::DELETE_ASSET, (DWORD_PTR)pair.second.Get(), };
				CTaskMgr::GetInst()->AddTask(task);
			}
		}
	}

	// Content 폴더안에 들어있는 모든 파일명 알아내기
	FindContentFile(CPathMgr::GetContentPath());

	// Content 폴더안에 있는 파일들을 에셋으로 로딩하기
	for (size_t i = 0; i < m_ContentFile.size(); ++i)
	{
		// 확장자 이름을 토대로 어떤 종류의 에셋인지 확인
		ASSET_TYPE Type = GetAssetType(m_ContentFile[i]);

		if (ASSET_END == Type)
			continue;
		
		// 확인된 타입에 맞게 파일을 메모리로 로딩
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
	// 즉시 확인
	DWORD dwStatus = WaitForSingleObject(m_NotifyHandle, 0);

	// 지정된 이벤트가 발생했다.
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