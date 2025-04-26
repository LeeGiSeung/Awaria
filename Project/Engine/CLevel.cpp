#include "pch.h"
#include "CLevel.h"

#include "CRenderMgr.h"

#include "CLayer.h"
#include "CGameObject.h"

CLevel::CLevel()
	: CEntity()
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer;
		m_Layer[i]->m_LayerIdx = i;
	}
}

CLevel::CLevel(const CLevel& _Origin)
	: CEntity(_Origin)
	, m_Layer{}
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = _Origin.m_Layer[i]->Clone();
	}	
}

CLevel::~CLevel()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		delete m_Layer[i];
	}
}

void CLevel::Begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->FinalTick();
	}
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	// 현재 상태와, 변경할 상태가 동일한 경우
	if (m_State == _NextState)
		return;

	// Stop -> Pause (X)
	if (m_State == LEVEL_STATE::STOP && _NextState == LEVEL_STATE::PAUSE)
		assert(nullptr);
	
	// Play -> Pause
	// Stop -> Play
	// Pause -> Play
	if (m_State == LEVEL_STATE::STOP && _NextState == LEVEL_STATE::PLAY)
	{		
		m_State = _NextState;
		Begin();
	}
	else
	{
		m_State = _NextState;
	}

	if (LEVEL_STATE::PLAY == m_State)
		CRenderMgr::GetInst()->SetRenderMode(true);
	else
		CRenderMgr::GetInst()->SetRenderMode(false);
}

void CLevel::Deregister()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->m_vecObjects.clear();
	}
}

void CLevel::AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_Layer[_LayerIdx]->AddGameObject(_Object, _bChildMove);
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj = m_Layer[i]->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetName() == _Name)
			{
				return vecObj[j];
			}
		}
	}

	return nullptr;
}
