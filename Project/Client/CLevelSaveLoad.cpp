#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CScript.h>

#include <Lecture/CScriptMgr.h>

void CLevelSaveLoad::SaveLevel(wstring _FilePath)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	
	// 레벨의 이름 저장	
	SaveWString(pCurLevel->GetName(), pFile);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		// Layer 이름 저장
		SaveWString(pLayer->GetName(), pFile);

		// Layer 에 속해있는 parentObject 정보 저장
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		// 오브젝트 개수 저장
		size_t ObjectCount = vecObjects.size();		
		fwrite(&ObjectCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < ObjectCount; ++j)
		{
			SaveGameObject(vecObjects[j], pFile);
		}
	}

	fclose(pFile);
}

CLevel* CLevelSaveLoad::LoadLevel(wstring _FilePath)
{
	CLevel* pNewLevel = new CLevel;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 레벨 이름 
	wstring LevelName;
	LoadWString(LevelName, pFile);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer 이름
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// 오브젝트 개수
		size_t ObjectCount = 0;
		fread(&ObjectCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < ObjectCount; ++i)
		{
			CGameObject* pObject = LoadGameObject(pFile);
			pLayer->AddGameObject(pObject, false);
		}
	}

	fclose(pFile);

	return pNewLevel;
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// 오브젝트 이름 저장
	SaveWString(_Object->GetName(), _File);

	// 컴포넌트 정보 저장
	COMPONENT_TYPE ComType = COMPONENT_TYPE::COMPONENT_END;

	for(UINT i = 0; i < COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pComponent)
			continue;

		ComType = pComponent->GetType();
		fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// 컴포넌트 이름 저장
		SaveWString(pComponent->GetName(), _File);

		// 해당 컴포넌트 정보 저장
		pComponent->SaveToFile(_File);
	}

	// 컴포넌트 정보 마감
	ComType = COMPONENT_TYPE::COMPONENT_END;
	fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

	// =================
	// 스크립트 정보 저장
	// =================
	// 스크립트 개수 저장
	const vector<CScript*>& vecScripts = _Object->GetScripts();	
	int Size = (int)vecScripts.size();
	fwrite(&Size, sizeof(int), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		// 스크립트의 종류(이름) 을 저장
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);	
		SaveWString(ScriptName, _File);

		// 스크립트의 정보 저장
		vecScripts[i]->SaveToFile(_File);
	}

	// =================
	// 자식 오브젝트 저장
	// =================
	const vector<CGameObject*>& vecChildObj = _Object->GetChild();
	int ChildCount = (int)vecChildObj.size();
	fwrite(&ChildCount, sizeof(int), 1, _File);

	for (size_t i = 0; i < vecChildObj.size(); ++i)
	{
		SaveGameObject(vecChildObj[i], _File);
	}
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	// 오브젝트 생성
	CGameObject* pNewObject = new CGameObject;

	// 오브젝트 이름
	wstring Name;
	LoadWString(Name, _File);
	pNewObject->SetName(Name);

	// 컴포넌트 정보
	while (true)
	{
		COMPONENT_TYPE ComType = COMPONENT_TYPE::COMPONENT_END;
		fread(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		if (COMPONENT_END == ComType)
			break;

		CComponent* pComponent = nullptr;

		switch (ComType)
		{
		case TRANSFORM:
			pComponent = new CTransform;
			break;
		case COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COLLIDER3D:
			//pComponent = new CCollider3D;
			break;
		case LIGHT2D:
			pComponent = new CLight2D;
			break;
		case LIGHT3D:
			//pComponent = new CLight3D;
			break;
		case CAMERA:
			pComponent = new CCamera;
			break;
		case BOUNDINGBOX:
			//pComponent = new CBoundingBox;
			break;
		case MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case SPRITERENDER:
			pComponent = new CSpriteRender;
			break;
		case FLIPBOOKRENDER:
			pComponent = new CFlipbookRender;
			break;
		case TILERENDER:
			pComponent = new CTileRender;
			break;
		case PARTICLERENDER:
			pComponent = new CParticleRender;
			break;
		case SKYBOX:
			//pComponent = new CSkyBox;
			break;
		case DECAL:
			//pComponent = new CDecal;
			break;
		case LANDSCAPE:
			//pComponent = new CLandScape;
			break;
		}

		// 컴포넌트 이름 불러오기
		wstring Name;
		LoadWString(Name, _File);
		pComponent->SetName(Name);

		// 생성한 컴포넌트를 오브젝트에 추가 및 저장데이터 로딩
		pNewObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// ====================
	// 스크립트 정보 불러오기
	// ====================
	// 스크립트 개수
	int Size = 0;
	fread(&Size, sizeof(int), 1, _File);

	for (size_t i = 0; i < Size; ++i)
	{
		// 스크립트의 종류(이름)
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// 스크립트 이름으로 스크립트를 생성시켜서 오브젝트에 넣어줌
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pNewObject->AddComponent(pScript);

		// 스크립트가 저장한 데이터를 다시 복구함
		pScript->LoadFromFile(_File);
	}

	// ====================
	// 자식 오브젝트 불러오기
	// ====================
	int ChildCount = 0;
	fread(&ChildCount, sizeof(int), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* pChildObj = LoadGameObject(_File);
		pNewObject->AddChild(pChildObj);
	}

	return pNewObject;
}
