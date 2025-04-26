#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer()
	: m_LayerIdx(-1)
{
}

CLayer::CLayer(const CLayer& _Origin)
	: CEntity(_Origin)	
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	for (size_t i = 0; i < _Origin.m_vecParentObjects.size(); ++i)
	{
		CGameObject* pCloneObj = _Origin.m_vecParentObjects[i]->Clone();
		AddGameObject(pCloneObj, false);
	}
}

CLayer::~CLayer()
{
	Delete_Vec(m_vecParentObjects);
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Begin();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_vecParentObjects.size(); ++i)
	{
		m_vecParentObjects[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	vector<CGameObject*>::iterator iter = m_vecParentObjects.begin();

	for (; iter != m_vecParentObjects.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
		{
			iter = m_vecParentObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CLayer::AddGameObject(CGameObject* _Parent, bool _bChildMove)
{
	m_vecParentObjects.push_back(_Parent);
	_Parent->m_LayerIdx = m_LayerIdx;

	// 자식을 보유했을 경우, 자식도 해당 레이어 소속으로 변경한다.
	static list<CGameObject*> queue;
	queue.clear();
	queue.push_back(_Parent);

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		const vector<CGameObject*>& vecChild = pObject->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}

		// 최상위 부모 오브젝트 
		// or 자식오브젝트여도, 자식까지 같이 이동하기로 한 경우 
		// or 최상위부모오브젝트x, 자식이동x 더라도 소속이 없는 오브젝트인 경우
		if (nullptr == pObject->GetParent() || _bChildMove || pObject->m_LayerIdx == -1)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}		
	}
}

void CLayer::DeregisterGameObjectAsParent(CGameObject* _Object)
{
	vector<CGameObject*>::iterator iter = m_vecParentObjects.begin();

	for (; iter != m_vecParentObjects.end(); ++iter)
	{
		if ((*iter) == _Object)
		{
			m_vecParentObjects.erase(iter);
			return;
		}
	}

	assert(nullptr);
}
