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
	// Task ó��
	// =========	
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		const tTask& task = m_vecTask[i];

		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			// ���� ������ �����´�.
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			// �Է����� ���� ������ ���� ������ �־��ش�.
			CGameObject* pNew = (CGameObject*)task.Param0;
			int LayerIdx = (int)task.Param1;
			pLevel->AddGameObject(pNew, LayerIdx, false);

			// Object �� ������ ������ Level �� ���°� Play or Pause ��
			// Begin �Լ��� ȣ�����ش�.
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

			// 1. Child �� ������ ������Ʈ�� �ٸ� ������Ʈ�� �ڽ��� ���
			//   - ���� �θ𿡼� ���ŵǾ�� ��
			if (pChild->GetParent())
			{
				pChild->DisconnectWithParent();

				if (nullptr == pParent)
					pChild->RegisterAsParent();
				else
					pParent->AddChild(pChild);

				m_LevelChanged = true;
			}

			// 2. Child �� ������ ������Ʈ�� �ֻ��� �θ��� ���
			//   - Layer ���� ����
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
