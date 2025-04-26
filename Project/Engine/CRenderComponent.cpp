#include "pch.h"
#include "CRenderComponent.h"

#include "CLevelMgr.h"
#include "CAssetMgr.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
	, m_CurMtrl(_Origin.m_SharedMtrl)
	, m_DynamicMtrl(nullptr)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_CurMtrl = m_SharedMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 현재 레벨의 상태가 Play 모드인지 확인
	assert(LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetLevelState());

	if (nullptr == m_DynamicMtrl)
	{
		m_DynamicMtrl = m_CurMtrl->Clone();
	}

	m_CurMtrl = m_DynamicMtrl;

	return m_DynamicMtrl;
}


void CRenderComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
	SaveAssetRef(m_CurMtrl, _File);
}

void CRenderComponent::LoadFromFile(FILE* _File)
{
	m_Mesh = LoadAssetRef<CMesh>(_File);
	m_SharedMtrl = LoadAssetRef<CMaterial>(_File);
	m_CurMtrl = LoadAssetRef<CMaterial>(_File);
}
