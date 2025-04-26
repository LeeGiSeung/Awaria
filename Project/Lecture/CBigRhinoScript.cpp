#include "pch.h"
#include "CBigRhinoScript.h"

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

CBigRhinoScript::CBigRhinoScript()
	:CScript(BIGRHINOSCRIPT)
	, m_Rhino_State(BIG_RHINO_STATE::BIG_RHINO_SPAWN)
	, TargetPos(0, 0, 0)
	, m_Speed(100.f)
	, MovingPos(0.f, 0.f)
	, Distance(0.f)
	, AttackPos(0.f, 0.f, 0.f)
	, AttackRhinoPos(0.f, 0.f, 0.f)
	, m_RhinoRDir(MOVE_DIR::MOVE_LEFT)
	, m_bBlockLeft(false)
	, m_bBlockRight(false)
	, m_bBlockUp(false)
	, m_bBlockDown(false)
	, bBlockCount(0)
	, CoolTime(0.f)
{
}

CBigRhinoScript::~CBigRhinoScript()
{
}

void CBigRhinoScript::Init()
{
}

void CBigRhinoScript::Begin()
{
}

void CBigRhinoScript::Tick()
{
	CoolTime += DT;

	CGameObject* m_CurObject = this->GetOwner();

	//큰 코뿔소가 소환되고 소환 애니메이션 끝났을때
	if (m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_SPAWN && m_CurObject->FlipbookRender()->IsFinish()) {
		m_Rhino_State = BIG_RHINO_STATE::BIG_RHINO_WALK;
		m_CurObject->FlipbookRender()->Play(BIG_RHINO_STATE::BIG_RHINO_WALK, 8, true);
	}

	//코끼리 현재 위치
	Vec3 BigRhinoPos = m_CurObject->Transform()->GetRelativePos();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(5); //PlayerLayer = 5
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	if (m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_WALK) {
		for (int j = 0; j < vecObj.size(); j++) {
			if (vecObj[j]->GetName() == L"pBody") {
				TargetPos = vecObj[j]->Transform()->GetWorldPos();
			}
		}
	}

	MovingPos.x = abs(BigRhinoPos.x - TargetPos.x);
	MovingPos.y = abs(BigRhinoPos.y - TargetPos.y);

	//Distance는 큰 코끼리와 Player와의 간격
	Distance = MovingPos.x + MovingPos.y;

	//##########################
	//코끼리 돌진 공격
	//##########################
	if (Distance < 300.f && m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_WALK && CoolTime > 2.f) {
		m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale());
		m_CurObject->FlipbookRender()->Play(BIG_RHINO_STATE::BIG_RHINO_ATTACK, 8, false);
		m_Rhino_State = BIG_RHINO_STATE::BIG_RHINO_ATTACK;
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_deer_slide_01 [1].wav");
		pSound->Play(1, 0.2f, false);

	}
	//코뿔소 돌진 공격이 끝나면 Walk 상태로 변환
	if (m_CurObject->FlipbookRender()->IsFinish() && m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_ATTACK) {
		m_Rhino_State = BIG_RHINO_STATE::BIG_RHINO_WALK;
		m_CurObject->FlipbookRender()->Play(BIG_RHINO_STATE::BIG_RHINO_WALK, 8, true);
		CoolTime = 0.f;
	}
	//코뿔소 돌진 공격 상태면 일정 Sprite에서 돌진함
	else if (m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_ATTACK) {
		//3~5
		int SpriteIdx = m_CurObject->FlipbookRender()->GetSpriteIdx();
		if (SpriteIdx >= 3 && SpriteIdx <= 5) {
			Vec2 dir = Vec2(TargetPos.x - BigRhinoPos.x, TargetPos.y - BigRhinoPos.y);
			if (dir.Length() > 1.f) {
				if (!m_bBlockRight && !m_bBlockLeft && !m_bBlockUp && !m_bBlockDown) {
					dir.Normalize();
					BigRhinoPos.x += dir.x * m_Speed * DT * 50;
					BigRhinoPos.y += dir.y * m_Speed * DT * 50;
				}
			}
		}
	}

	//코뿔소 걷는 상태면 Player쪽으로 감
	if (m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_WALK) {
		//m_CoolTime += DT;
		//m_TakeDownCoolTime += DT;
		if (BigRhinoPos.x - TargetPos.x < 0) {
			BigRhinoPos.x += DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(1);
			m_RhinoRDir = MOVE_DIR::MOVE_RIGHT;
		}
		else {
			BigRhinoPos.x -= DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(0);
			m_RhinoRDir = MOVE_DIR::MOVE_LEFT;
		}

		if (BigRhinoPos.y - TargetPos.y < 0) {
			BigRhinoPos.y += DT * m_Speed;
			m_RhinoRDir = MOVE_DIR::MOVE_DOWN;
		}
		else{
			BigRhinoPos.y -= DT * m_Speed;
			m_RhinoRDir = MOVE_DIR::MOVE_UP;
		}
	}

	m_CurObject->Transform()->SetRelativePos(BigRhinoPos);
}

void CBigRhinoScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m = this->GetOwner();

	if (_Other->GetName() == L"pWall") {
		bBlockCount += 1;
		Vec2 playerColScale = this->Collider2D()->GetScale();
		Vec2 wallColScale = _Other->Collider2D()->GetScale();

		Vec3 playerPos = this->Transform()->GetRelativePos();
		Vec3 wallPos = _Other->Transform()->GetRelativePos();

		// 중심 좌표 거리
		Vec2 delta = Vec2(playerPos.x - wallPos.x, playerPos.y - wallPos.y);
		// 충돌 겹침 정도 계산 (각 축별)
		float overlapX = (playerColScale.x / 2 + wallColScale.x / 2) - abs(delta.x);
		float overlapY = (playerColScale.y / 2 + wallColScale.y / 2) - abs(delta.y);

		if (overlapX < overlapY) {
			// X축 충돌이 더 큼
			if (delta.x < 0) {
				m_bBlockRight = true;  // 벽이 오른쪽
			}
			else {
				m_bBlockLeft = true;   // 벽이 왼쪽
			}
		}
		else {
			// Y축 충돌이 더 큼
			if (delta.y < 0) {
				m_bBlockUp = true;  // 벽이 아래쪽
			}
			else {
				m_bBlockDown = true;    // 벽이 위쪽
			}
		}
	}

	//코뿔소와 Player가 충돌했을 당시
	if (_Other->GetName() == L"pBody") {
		//코뿔소가 공격 상태일때
		if (m_Rhino_State == BIG_RHINO_STATE::BIG_RHINO_ATTACK) {
			int SpriteIdx = FlipbookRender()->GetSpriteIdx();

			//스프라이트도 일정 기간 이내일때
			if (SpriteIdx >= 3 && SpriteIdx <= 5) {
				//플레이어 Heat 처리
				_Other->SetPlayerHeat(true);
			}
		}
	}

}

void CBigRhinoScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{

}

void CBigRhinoScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m = this->GetOwner();

	wstring OtherName = _Other->GetName();
	if (OtherName == L"pWall") {
		//bBlockCount -= 1;
		m_bBlockRight = false;
		m_bBlockLeft = false;
		m_bBlockDown = false;
		m_bBlockUp = false;

	}
}

void CBigRhinoScript::SaveToFile(FILE* _File)
{
}

void CBigRhinoScript::LoadFromFile(FILE* _File)
{
}


