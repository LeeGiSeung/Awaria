#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CAssetMgr.h"
#include "CAsset.h"

CTaskMgr::CTaskMgr()
	: m_LevelChanged(false)
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	m_LevelChanged = false;

	for (size_t i = 0; i < m_vecGC.size(); ++i)
	{
		delete m_vecGC[i];
	}
	m_vecGC.clear();

	// =========
	// Task 처리
	// =========	
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];

		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			// 현재 레벨을 가져온다.
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			// 입력으로 들어온 레벨을 현재 레벨에 넣어준다.
			CGameObject* pNew = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pLevel->AddGameObject(pNew, LayerIdx, false);

			// Object 가 생성된 시점에 Level 의 상태가 Play or Pause 면
			// Begin 함수를 호출해준다.
			if (pLevel->GetState() != LEVEL_STATE::STOP)
			{
				pNew->Begin();
			}
			
			m_LevelChanged = true;
		}
			break;

		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObj = (CGameObject*)task.Param0;

			if (!pObj->m_Dead)
			{
				pObj->m_Dead = true;
				m_vecGC.push_back(pObj);
			}

			m_LevelChanged = true;
		}
			break;
		case TASK_TYPE::ADD_CHILD:
		{
			CGameObject* pParent = (CGameObject*)task.Param0;
			CGameObject* pChild = (CGameObject*)task.Param1;

			// 1. Child 로 지정된 오브젝트가 다른 오브젝트의 자식인 경우
			//   - 기존 부모에서 제거되어야 함
			if (pChild->GetParent())
			{
				pChild->DisconnectWithParent();

				if (nullptr == pParent)
					pChild->RegisterAsParent();
				else
					pParent->AddChild(pChild);

				m_LevelChanged = true;
			}

			// 2. Child 로 지정된 오브젝트가 최상위 부모인 경우
			//   - Layer 에서 제거
			else
			{
				if (pParent)
				{
					pChild->DisconnectWithLayerAsParent();
					pParent->AddChild(pChild);
					m_LevelChanged = true;
				}
			}
		}
			break;

		case TASK_TYPE::CHANGE_LEVEL_STATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param0;
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeState(NextState);
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevel* pNextLevel = (CLevel*)task.Param0;

			CLevelMgr::GetInst()->ChangeLevel(pNextLevel);

			m_LevelChanged = true;
		}
			break;	

		case TASK_TYPE::DELETE_ASSET:
		{
			Ptr<CAsset> pAsset = (CAsset*)task.Param0;
			CAssetMgr::GetInst()->DeleteAsset(pAsset);
		}
			break;
		}
	}

	m_vecTask.clear();
}
