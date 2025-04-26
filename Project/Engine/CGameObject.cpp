#include "pch.h"
#include "CGameObject.h"
#include "components.h"
#include "CScript.h"

#include "CRenderComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"



CGameObject::CGameObject()
	: m_Com{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1) 
	, m_Dead(false)
	, UISpawnPart(false)
	, Tag(false)
	, GeneratorUIUpdate(false)
	, GeneratorCheck1(false)
	, GeneratorCheck2(false)
	, SpawnPart(false)
	, want(false)
	, App_state(APPARATUS_END)
	, m_Generator_ui_state(GENERATOR_UI_STATE::GENERATOR_UI_QUESTION)
{
}

CGameObject::CGameObject(const CGameObject& _Origin)
	: CEntity(_Origin)
	, m_Com{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(_Origin.m_LayerIdx)
	, m_Dead(false)
{
	// 컴포넌트 복제
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr == _Origin.m_Com[i])
			continue;

		AddComponent(_Origin.m_Com[i]->Clone());		
	}

	// 스크립트 복제
	for (size_t i = 0; i < _Origin.m_vecScript.size(); ++i)
	{
		AddComponent(_Origin.m_vecScript[i]->Clone());		
	}
		
	// 자식 오브젝트 복제
	for (size_t i = 0; i < _Origin.m_vecChild.size(); ++i)
	{
		AddChild(_Origin.m_vecChild[i]->Clone());
	}
}



CGameObject::~CGameObject()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			delete m_Com[i];
	}
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

bool CGameObject::IsAncestor(CGameObject* _Object)
{	
	CGameObject* pParent = GetParent();

	while (pParent)
	{
		if (_Object == pParent)
			return true;

		pParent = pParent->GetParent();
	}

	return false;
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE Type = _Component->GetType();

	if (Type == SCRIPT)
	{
		m_vecScript.push_back((CScript*)_Component);
	}
	else
	{
		// 이미 가지고 있는 컴포넌트인 경우
		assert(!m_Com[Type]);
		m_Com[Type] = _Component;

		if (dynamic_cast<CRenderComponent*>(_Component))
		{
			// 하나의 GameObject 는 한종류의 RenderComonent 만 가질 수 있다.
			if (nullptr != m_RenderCom)
				assert(nullptr);

			m_RenderCom = (CRenderComponent*)_Component;
		}		
	}

	_Component->m_Owner = this;


	// 컴포넌트 초기화
	_Component->Init();
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != m_Com[i])
			m_Com[i]->FinalTick();
	}

	// Layer 에 GameObject 등록하기
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pMyLayer = pCurLevel->GetLayer(m_LayerIdx);
	pMyLayer->RegisterGameObject(this);

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChild.erase(iter);
		else
			++iter;
	}
}

void CGameObject::Render()
{
	if (!m_RenderCom)
		return;

	m_RenderCom->Render();
}

void CGameObject::DisconnectWithParent()
{
	// 부모가 있어야 가능
	assert(m_Parent);

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	// 자식이 부모로 지정한 오브젝트가 자식 오브젝트의 주소를 모르는 경우
	assert(nullptr);
}

void CGameObject::RegisterAsParent()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pCurLayer = pLevel->GetLayer(m_LayerIdx);

	pCurLayer->AddGameObject(this, false);
}

void CGameObject::DisconnectWithLayerAsParent()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pCurLayer = pLevel->GetLayer(m_LayerIdx);
		
	pCurLayer->DeregisterGameObjectAsParent(this);
}

void CGameObject::AddChild(CGameObject* _Child)
{
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}


#include "CTaskMgr.h"
void CGameObject::Destroy()
{
	tTask task = {};

	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param0 = (DWORD_PTR)this;

	CTaskMgr::GetInst()->AddTask(task);
}