#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CAssetMgr.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	delete m_CurLevel;
}

LEVEL_STATE CLevelMgr::GetLevelState()
{
	return m_CurLevel->GetState();
}

void CLevelMgr::Init()
{
	m_CurLevel = new CLevel;
}

void CLevelMgr::Progress()
{
	// =====
	// Tick
	// =====
	m_CurLevel->Tick();

	m_CurLevel->Deregister();

	m_CurLevel->FinalTick();
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	assert(m_CurLevel != _NextLevel);

	delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// ������ ����Ǹ鼭, RenderMgr �� ����ִ� ������ ������ ī�޶� ���� ������Ų��.
	CRenderMgr::GetInst()->ClearCamera();
}