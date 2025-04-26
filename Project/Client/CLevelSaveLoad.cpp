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
	
	// ������ �̸� ����	
	SaveWString(pCurLevel->GetName(), pFile);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);

		// Layer �̸� ����
		SaveWString(pLayer->GetName(), pFile);

		// Layer �� �����ִ� parentObject ���� ����
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		// ������Ʈ ���� ����
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

	// ���� �̸� 
	wstring LevelName;
	LoadWString(LevelName, pFile);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer �̸�
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// ������Ʈ ����
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
	// ������Ʈ �̸� ����
	SaveWString(_Object->GetName(), _File);

	// ������Ʈ ���� ����
	COMPONENT_TYPE ComType = COMPONENT_TYPE::COMPONENT_END;

	for(UINT i = 0; i < COMPONENT_TYPE::COMPONENT_END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pComponent)
			continue;

		ComType = pComponent->GetType();
		fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		// ������Ʈ �̸� ����
		SaveWString(pComponent->GetName(), _File);

		// �ش� ������Ʈ ���� ����
		pComponent->SaveToFile(_File);
	}

	// ������Ʈ ���� ����
	ComType = COMPONENT_TYPE::COMPONENT_END;
	fwrite(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

	// =================
	// ��ũ��Ʈ ���� ����
	// =================
	// ��ũ��Ʈ ���� ����
	const vector<CScript*>& vecScripts = _Object->GetScripts();	
	int Size = (int)vecScripts.size();
	fwrite(&Size, sizeof(int), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		// ��ũ��Ʈ�� ����(�̸�) �� ����
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);	
		SaveWString(ScriptName, _File);

		// ��ũ��Ʈ�� ���� ����
		vecScripts[i]->SaveToFile(_File);
	}

	// =================
	// �ڽ� ������Ʈ ����
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
	// ������Ʈ ����
	CGameObject* pNewObject = new CGameObject;

	// ������Ʈ �̸�
	wstring Name;
	LoadWString(Name, _File);
	pNewObject->SetName(Name);

	// ������Ʈ ����
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

		// ������Ʈ �̸� �ҷ�����
		wstring Name;
		LoadWString(Name, _File);
		pComponent->SetName(Name);

		// ������ ������Ʈ�� ������Ʈ�� �߰� �� ���嵥���� �ε�
		pNewObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// ====================
	// ��ũ��Ʈ ���� �ҷ�����
	// ====================
	// ��ũ��Ʈ ����
	int Size = 0;
	fread(&Size, sizeof(int), 1, _File);

	for (size_t i = 0; i < Size; ++i)
	{
		// ��ũ��Ʈ�� ����(�̸�)
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// ��ũ��Ʈ �̸����� ��ũ��Ʈ�� �������Ѽ� ������Ʈ�� �־���
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pNewObject->AddComponent(pScript);

		// ��ũ��Ʈ�� ������ �����͸� �ٽ� ������
		pScript->LoadFromFile(_File);
	}

	// ====================
	// �ڽ� ������Ʈ �ҷ�����
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
