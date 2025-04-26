#include "pch.h"
#include "CSkulScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>
#include "CMissileScript.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbookRender.h>

CSkulScript::CSkulScript()
	:CScript(SKULSCRIPT)
	, m_Skul_State(SKUL_STATE::SKUL_SPAWN)
	, m_Speed(500.f)
	, Distance(0.f)
{
}

CSkulScript::~CSkulScript()
{
}

void CSkulScript::Init()
{
}

void CSkulScript::Begin()
{
}

void CSkulScript::Tick()
{
	CGameObject* m_CurObject = this->GetOwner();
	Vec3 SkulPos = m_CurObject->Transform()->GetRelativePos();

	if (m_Skul_State == SKUL_STATE::SKUL_SPAWN && m_CurObject->FlipbookRender()->IsFinish()) {
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_skeleton_guy_spawn_01 [1].wav");
		pSound->Play(1, 0.2f, false);

		m_Skul_State = SKUL_STATE::SKUL_READY;
		m_CurObject->FlipbookRender()->Play(SKUL_STATE::SKUL_READY, 8, false);
	}
	else if (m_Skul_State == SKUL_STATE::SKUL_READY && m_CurObject->FlipbookRender()->IsFinish())
	{
		m_Skul_State = SKUL_STATE::SKUL_RUN;
		m_CurObject->FlipbookRender()->Play(SKUL_STATE::SKUL_RUN, 8, true);
	}
	else if (m_Skul_State == SKUL_STATE::SKUL_DEATH && m_CurObject->FlipbookRender()->IsFinish())
	{
		m_CurObject->Destroy();
	}

	if (m_Skul_State == SKUL_STATE::SKUL_RUN) {
		if (m_CurObject->GetName() == L"SkulRight") {
			SkulPos.x += DT * m_Speed;
		}
		else if (m_CurObject->GetName() == L"SkulLeft") {
			SkulPos.x -= DT * m_Speed;
		}
	}

	if (m_Skul_State != SKUL_STATE::SKUL_DEATH) {
		m_CurObject->Transform()->SetRelativePos(SkulPos);
	}

}

void CSkulScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	if (OtherName == L"pWall") {
		m_Skul_State = SKUL_STATE::SKUL_DEATH;
		CGameObject* m_CurObject = this->GetOwner();
		m_CurObject->FlipbookRender()->Play(SKUL_STATE::SKUL_DEATH, 8, false);
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_skull_death_01 [1].wav");
		pSound->Play(1, 0.2f, false);

	}
	if (m_Skul_State == SKUL_STATE::SKUL_RUN) {
		//코뿔소와 Player가 충돌했을 당시
		if (_Other->GetName() == L"pBody") {
			//코뿔소가 공격 상태일때
				//플레이어 Heat 처리
			_Other->SetPlayerHeat(true);
		}
	}
}

void CSkulScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CSkulScript::SaveToFile(FILE* _File)
{
}

void CSkulScript::LoadFromFile(FILE* _File)
{
}

