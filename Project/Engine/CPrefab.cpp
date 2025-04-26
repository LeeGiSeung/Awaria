#include "pch.h"
#include "CPrefab.h"

#include "CGameObject.h"
#include "CPathMgr.h"

GAMEOBJECT_SAVE  CPrefab::g_Save = nullptr;
GAMEOBJECT_LOAD  CPrefab::g_Load = nullptr;

CPrefab::CPrefab(bool _EngineAsset)
	: CAsset(ASSET_TYPE::PREFAB, _EngineAsset)
	, m_PrefabObject(nullptr)
{
}

CPrefab::~CPrefab()
{
	if (nullptr != m_PrefabObject)
		delete m_PrefabObject;
}

void CPrefab::SetGameObject(CGameObject* _Object)
{
	m_PrefabObject = _Object;

	// 프리팹으로 전환하려는 오브젝트가 레벨안에 존재하고 있다면
	if (-1 != _Object->GetLayerIndex())
	{
		m_PrefabObject->DisconnectWithLayerAsParent();
		m_PrefabObject->m_LayerIdx = -1;
	}
}

CGameObject* CPrefab::Instantiate()
{
	return m_PrefabObject->Clone();
}

int CPrefab::Save(const wstring& _FilePath)
{

	//wstring RelativePath = L"Prefab\\" + _FilePath;
	//CheckExt(L".pref", RelativePath);

	//// 에셋이 자신이 어디에 저장되는지를 알게 함
	////SetRelativePath(RelativePath);

	wstring CurPath = _FilePath;
	CurPath.erase(std::find(CurPath.begin(), CurPath.end(), L'\0'), CurPath.end());

	int a = CurPath.size();

	wstring strFilePath = CPathMgr::GetContentPath() + CurPath;

	strFilePath += L".pref";
	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	//g_Save(m_PrefabObject, pFile);

	//fclose(pFile);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	g_Save(m_PrefabObject, pFile);

	fclose(pFile);

	return S_OK;
}

int CPrefab::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 파일 스트림 열기 실패 시
	if (nullptr == pFile)
		return E_FAIL;

	m_PrefabObject = g_Load(pFile);

	fclose(pFile);

	return S_OK;
}
