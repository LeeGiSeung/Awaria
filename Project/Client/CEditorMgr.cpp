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
	// Editor 용 카메라 제작
	CGameObjectEX* pCamObj = new CGameObjectEX;
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CEditorCamScript);

	pCamObj->Camera()->CheckLayerAll();
	//에디터 용 카메라 일단 직각 투영으로 하자잉
	//pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetFar(100000.f);

	m_vecEditorObj.push_back(pCamObj);

	// RenderMgr 에 Editor 카메라 등록
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


