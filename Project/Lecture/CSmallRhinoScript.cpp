#include "pch.h"
#include "CSmallRhinoScript.h"

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

CSmallRhinoScript::CSmallRhinoScript()
	:CScript(SMALLRHINOSCRIPT)
	, m_Rhino_State(SMALL_RHINO_STATE::SMALL_RHINO_SPAWN)
	, TargetPos(0, 0, 0)
	, m_Speed(100.f)
	, MovingPos(0.f, 0.f)
	, Distance(0.f)
	, CoolTime(0.f)
	, AttackPos(0.f, 0.f, 0.f)
	, AttackRhinoPos(0.f, 0.f, 0.f)
{
}

CSmallRhinoScript::~CSmallRhinoScript()
{
}

void CSmallRhinoScript::Init()
{
}

void CSmallRhinoScript::Begin()
{
}

void CSmallRhinoScript::Tick()
{
	CoolTime += DT;
	CGameObject* m_CurObject = this->GetOwner();

	//ū �ڻԼҰ� ��ȯ�ǰ� ��ȯ �ִϸ��̼� ��������
	if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_SPAWN && m_CurObject->FlipbookRender()->IsFinish()) {
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_deer_small_spawn_01 [1].wav");
		pSound->Play(1, 0.2f, false);

		m_Rhino_State = SMALL_RHINO_STATE::SMALL_RHINO_WALK;
		m_CurObject->FlipbookRender()->Play(SMALL_RHINO_STATE::SMALL_RHINO_WALK, 8, true);
	}

	//�ڳ��� ���� ��ġ
	Vec3 BigRhinoPos = m_CurObject->Transform()->GetRelativePos();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(5); //PlayerLayer = 5
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_WALK) {
		for (int j = 0; j < vecObj.size(); j++) {
			if (vecObj[j]->GetName() == L"pBody") {
				TargetPos = vecObj[j]->Transform()->GetWorldPos();
			}
		}
	}

	MovingPos.x = abs(BigRhinoPos.x - TargetPos.x);
	MovingPos.y = abs(BigRhinoPos.y - TargetPos.y);

	//Distance�� ū �ڳ����� Player���� ����
	Distance = MovingPos.x + MovingPos.y;

	//##########################
	//�ڳ��� ���� ����
	//##########################
	if (Distance < 300.f && m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_WALK && CoolTime > 1.f) {
		m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale());
		m_CurObject->FlipbookRender()->Play(SMALL_RHINO_STATE::SMALL_RHINO_ATTACK, 8, false);
		m_Rhino_State = SMALL_RHINO_STATE::SMALL_RHINO_ATTACK;
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_deer_small_attack_01 [1].wav");
		pSound->Play(1, 0.2f, false);

	}
	//�ڻԼ� ���� ������ ������ Walk ���·� ��ȯ
	if (m_CurObject->FlipbookRender()->IsFinish() && m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_ATTACK) {
		m_Rhino_State = SMALL_RHINO_STATE::SMALL_RHINO_WALK;
		m_CurObject->FlipbookRender()->Play(SMALL_RHINO_STATE::SMALL_RHINO_WALK, 8, true);
		CoolTime = 0.f;
	}
	//�ڻԼ� ���� ���� ���¸� ���� Sprite���� ������
	else if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_ATTACK) {
		//3~5
		int SpriteIdx = m_CurObject->FlipbookRender()->GetSpriteIdx();
		if (SpriteIdx >= 3 && SpriteIdx <= 5) {
			Vec2 dir = Vec2(TargetPos.x - BigRhinoPos.x, TargetPos.y - BigRhinoPos.y);
			if (dir.Length() > 1.f) {
				dir.Normalize();
				BigRhinoPos.x += dir.x * m_Speed * DT * 50;
				BigRhinoPos.y += dir.y * m_Speed * DT * 50;
			}
		}
	}

	//�ڻԼ� �ȴ� ���¸� Player������ ��
	if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_WALK) {
		//m_CoolTime += DT;
		//m_TakeDownCoolTime += DT;
		if (BigRhinoPos.x - TargetPos.x < 0) {
			BigRhinoPos.x += DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(1);
		}
		else {
			BigRhinoPos.x -= DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(0);
		}

		if (BigRhinoPos.y - TargetPos.y < 0) {
			BigRhinoPos.y += DT * m_Speed;
		}
		else {
			BigRhinoPos.y -= DT * m_Speed;
		}
	}

	if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_DEATH && m_CurObject->FlipbookRender()->IsFinish()) {
		m_CurObject->Destroy();
	}

	m_CurObject->Transform()->SetRelativePos(BigRhinoPos);
}

void CSmallRhinoScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	if (OtherName == L"pWall") {
		m_Rhino_State = SMALL_RHINO_STATE::SMALL_RHINO_DEATH;
		CGameObject* m_CurObject = this->GetOwner();
		m_CurObject->FlipbookRender()->Play(SMALL_RHINO_STATE::SMALL_RHINO_DEATH, 8, false);

		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_skull_death_01 [1].wav");
		pSound->Play(1, 0.2f, false);
	}

	//�ڻԼҿ� Player�� �浹���� ���
	if (_Other->GetName() == L"pBody") {
		//�ڻԼҰ� ���� �����϶�
		if (m_Rhino_State == SMALL_RHINO_STATE::SMALL_RHINO_ATTACK) {
			int SpriteIdx = FlipbookRender()->GetSpriteIdx();

			//��������Ʈ�� ���� �Ⱓ �̳��϶�
			if (SpriteIdx >= 3 && SpriteIdx <= 5) {
				//�÷��̾� Heat ó��
				_Other->SetPlayerHeat(true);
			}
		}
	}

}

void CSmallRhinoScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CSmallRhinoScript::SaveToFile(FILE* _File)
{
}

void CSmallRhinoScript::LoadFromFile(FILE* _File)
{
}

