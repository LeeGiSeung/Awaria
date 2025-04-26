#include "pch.h"
#include "CCollider2D.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"
#include "CCollisionMgr.h"
#include "CScript.h"


CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Scale(Vec3(1.f, 1.f, 0.f))	
	, m_OverlapCount(0)
	, m_IndependentScale(false)
	, m_NotifyParent(false)
	, m_ShowColor(true)
{
}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: CComponent(_Origin)
	, m_Scale(_Origin.m_Scale)
	, m_Offset(_Origin.m_Offset)
	, m_OverlapCount(0)
	, m_IndependentScale(_Origin.m_IndependentScale)
	, m_NotifyParent(_Origin.m_NotifyParent)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::FinalTick()
{
	m_ShowColor = CCollisionMgr::GetInst()->GetShowCollider();

	if (CCollisionMgr::GetInst()->GetCanChangeShowColor() == true) {
		if (KEY_TAP(KEY::Q)) {
			if (m_ShowColor) {
				CCollisionMgr::GetInst()->SetShowCollider(false);
			}
			else {
				CCollisionMgr::GetInst()->SetShowCollider(true);
			}
			m_ShowColor = CCollisionMgr::GetInst()->GetShowCollider();
			CCollisionMgr::GetInst()->SetCanChangeShowColor(false);
		}
	}
	else {
		CCollisionMgr::GetInst()->SetShowColorTime(DT);
	}

	if (CCollisionMgr::GetInst()->GetShowColorTime() > 2.f) {
		CCollisionMgr::GetInst()->SetCanChangeShowColor(true);
		CCollisionMgr::GetInst()->SetShowColorTime(0.f);
	}
	

	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	Matrix matTrans = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
	m_matColWorld = matScale * matTrans;

	if (m_IndependentScale)
	{
		Vec3 Scale = Transform()->GetWorldScale();
		Matrix matScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(Scale.x, Scale.y, Scale.z));
		m_matColWorld = m_matColWorld * matScaleInv * Transform()->GetWorldMat();
	}
	else
	{
		m_matColWorld *= Transform()->GetWorldMat();
	}

	// DebugRender 로 충돌체의 위치를 표시함
	// 다른 충돌체와 겹쳐있으면 Red, 아무 충돌체하고 겹쳐있지 않으면 Green
	Vec4 vColor;
	if (m_ShowColor) {
		if (m_OverlapCount != 0)
			vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		else {
			if (this->GetOwner()->GetName() == L"pWall") {
				vColor = Vec4(0.f, 1.f, 1.f, 1.f);
			}
			else if (this->GetOwner()->GetName() == L"BigRhino" ||
				this->GetOwner()->GetName() == L"SmallRhino" ||
				this->GetOwner()->GetName() == L"Scrap" ||
				this->GetOwner()->GetName() == L"SkulLeft" ||
				this->GetOwner()->GetName() == L"SkulRight" ||
				this->GetOwner()->GetName() == L"Zmora"||
				this->GetOwner()->GetName() == L"BombSkul"
				) {
				vColor = Vec4(1.f, 1.f, 0.f, 1.f);
			}
			else {
				vColor = Vec4(0.f, 1.f, 0.f, 1.f);
			}
		}
	}

	DrawDebugRect(vColor, m_matColWorld, false);
}


void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	++m_OverlapCount;
	NotifyBeginOverlap(_Other);
}

void CCollider2D::Overlap(CCollider2D* _Other)
{
	NotifyOverlap(_Other);
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	--m_OverlapCount;
	NotifyEndOverlap(_Other);
}

void CCollider2D::NotifyBeginOverlap(CCollider2D* _Other)
{
	CGameObject* pObject = GetOwner();

	const vector<CScript*>& vecScripts = pObject->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
	}

	if (m_NotifyParent)
	{
		pObject = pObject->GetParent();

		while (pObject)
		{
			const vector<CScript*>& vecScripts = pObject->GetScripts();
			for (size_t i = 0; i < vecScripts.size(); ++i)
			{
				vecScripts[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
			}
			pObject = pObject->GetParent();
		}
	}
}

void CCollider2D::NotifyOverlap(CCollider2D* _Other)
{
	CGameObject* pObject = GetOwner();

	const vector<CScript*>& vecScripts = pObject->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->Overlap(this, _Other->GetOwner(), _Other);
	}

	if (m_NotifyParent)
	{
		pObject = pObject->GetParent();

		while (pObject)
		{
			const vector<CScript*>& vecScripts = pObject->GetScripts();
			for (size_t i = 0; i < vecScripts.size(); ++i)
			{
				vecScripts[i]->Overlap(this, _Other->GetOwner(), _Other);
			}
			pObject = pObject->GetParent();
		}
	}
}

void CCollider2D::NotifyEndOverlap(CCollider2D* _Other)
{
	CGameObject* pObject = GetOwner();

	const vector<CScript*>& vecScripts = pObject->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->EndOverlap(this, _Other->GetOwner(), _Other);
	}

	if (m_NotifyParent)
	{
		pObject = pObject->GetParent();

		while (pObject)
		{
			const vector<CScript*>& vecScripts = pObject->GetScripts();
			for (size_t i = 0; i < vecScripts.size(); ++i)
			{
				vecScripts[i]->EndOverlap(this, _Other->GetOwner(), _Other);
			}
			pObject = pObject->GetParent();
		}
	}
}


void CCollider2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Scale, sizeof(Vec2), 1, _File);
	fwrite(&m_Offset, sizeof(Vec2), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
	fwrite(&m_NotifyParent, sizeof(bool), 1, _File);
}

void CCollider2D::LoadFromFile(FILE* _File)
{
	fread(&m_Scale, sizeof(Vec2), 1, _File);
	fread(&m_Offset, sizeof(Vec2), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
	fread(&m_NotifyParent, sizeof(bool), 1, _File);
}