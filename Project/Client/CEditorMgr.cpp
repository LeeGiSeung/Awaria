#include "pch.h"
#include "CEditorMgr.h"

#include "CGameObjectEX.h"
#include <Engine/components.h>
#include "CEditorCamScript.h"

#include <Engine/CRenderMgr.h>

CEditorMgr::CEditorMgr()
{

}

CEditorMgr::~CEditorMgr()
{
	Delete_Vec(m_vecEditorObj);
}

void CEditorMgr::Init()
{
	// Editor �� ī�޶� ����
	CGameObjectEX* pCamObj = new CGameObjectEX;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CEditorCamScript);

	pCamObj->Camera()->CheckLayerAll();
	//������ �� ī�޶� �ϴ� ���� �������� ������
	//pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetFar(100000.f);

	m_vecEditorObj.push_back(pCamObj);

	// RenderMgr �� Editor ī�޶� ���
	CRenderMgr::GetInst()->RegisterEditorCamera(pCamObj->Camera());
}

void CEditorMgr::Progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->Tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->FinalTick();
	}
}


