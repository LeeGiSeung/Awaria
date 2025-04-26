#include "pch.h"
#include "CZmoraScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>
#include <Engine/CFlipbookRender.h>

#include <Engine/CLayer.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

CZmoraScript::CZmoraScript()
	:CScript(ZMORASCRIPT)
	, m_ZmoraState(ZMORA_STATE::ZMORA_SPAWN)
	, m_Speed(100.f)
	, TargetPos(0, 0, 0)
	, m_CoolTime(0.f)
	, m_TakeDownCoolTime(0.f)
	, m_bBlockDown(false)
	, m_bBlockUp(false)
	, m_bBlockLeft(false)
	, m_bBlockRight(false)
{
}

CZmoraScript::~CZmoraScript()
{
}

void CZmoraScript::Init()
{
}

void CZmoraScript::Begin()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_zmora_spawn_01 [1].wav");
	pSound->Play(1, 0.2f, false);
}

void CZmoraScript::Tick()
{
	CGameObject* m_CurObject = this->GetOwner();

	//Zmora ���� ��ġ
	Vec3 ZmoraPos = m_CurObject->Transform()->GetRelativePos();

	Vec2 MovingPos;
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(5); //PlayerLayer = 5
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	CGameObject* pWant = new CGameObject;
	Ptr<CMaterial> pMtrl;

	CGameObject* pWant2 = new CGameObject;
	Ptr<CMaterial> pMtrl2;

	if (m_ZmoraState == ZMORA_STATE::ZMORA_WALK) {
		for (int j = 0; j < vecObj.size(); j++) {
			if (vecObj[j]->GetName() == L"pBody") {
				TargetPos = vecObj[j]->Transform()->GetWorldPos();
			}
		}
	}

	MovingPos.x = abs(ZmoraPos.x - TargetPos.x);
	MovingPos.y = abs(ZmoraPos.y - TargetPos.y);

	float Distance = MovingPos.x + MovingPos.y;
	if (m_ZmoraState == ZMORA_STATE::ZMORA_SPAWN && m_CurObject->FlipbookRender()->IsFinish()) {
		m_ZmoraState = ZMORA_STATE::ZMORA_WALK;
		m_CurObject->FlipbookRender()->Play(ZMORA_STATE::ZMORA_WALK, 10, true);
		m_CurObject->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	}

	//Zmora ���� Swing -> Walk���� ��ȯ
	if (m_ZmoraState == ZMORA_STATE::ZMORA_SWING && m_CurObject->FlipbookRender()->IsFinish()) {
		m_ZmoraState = ZMORA_STATE::ZMORA_WALK;
		m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale() - Vec3(100.f, 100.f, 0.f));
		m_CurObject->FlipbookRender()->Play(ZMORA_STATE::ZMORA_WALK, 10, true);
		ResetCoolTime();
	}
	//Zmora ���� TakeDown -> Walk���� ��ȯ
	if (m_ZmoraState == ZMORA_STATE::ZMORA_TAKEDOWN && m_CurObject->FlipbookRender()->IsFinish()) {
		m_ZmoraState = ZMORA_STATE::ZMORA_WALK;
		m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale() - Vec3(100.f, 100.f, 0.f));
		m_CurObject->FlipbookRender()->Play(ZMORA_STATE::ZMORA_WALK, 10, true);
		ResetCoolTime();
	}

	//Zmora ���� Swing���� ��ȯ
	if (m_CoolTime > 0.5f) {
		//ZmoraSwing ����
		if (Distance < 300.f && m_ZmoraState == ZMORA_STATE::ZMORA_WALK) {
			m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale() + Vec3(100.f, 100.f, 0.f));
			m_CurObject->FlipbookRender()->Play(ZMORA_STATE::ZMORA_SWING, 12, false);
			m_ZmoraState = ZMORA_STATE::ZMORA_SWING;
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_zmora_attack_dash_01 [1].wav");
			pSound->Play(1, 0.2f, false);

		}
	}

	//Zmora ���� TakeDown���� ��ȯ
	if (m_TakeDownCoolTime > 2.f) {
		//ZmoraTakeDown ����
		if (Distance > 300.f && m_ZmoraState == ZMORA_STATE::ZMORA_WALK) {
			ZmoraPos = TargetPos;
			m_CurObject->Transform()->SetRelativeScale(m_CurObject->Transform()->GetRelativeScale() + Vec3(100.f, 100.f, 0.f));
			m_CurObject->FlipbookRender()->Play(ZMORA_STATE::ZMORA_TAKEDOWN, 12, false);
			m_ZmoraState = ZMORA_STATE::ZMORA_TAKEDOWN;
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_zmora_attack_ground_hit_01 [1].wav");
			pSound->Play(1, 0.2f, false);
		}
	}

	if (m_ZmoraState == ZMORA_STATE::ZMORA_SWING) {
		//index�� 4���� 9���� Swing
		if (m_CurObject->FlipbookRender()->GetSpriteIdx() >= 4 && m_CurObject->FlipbookRender()->GetSpriteIdx() <= 9) {
			Vec2 dir = Vec2(TargetPos.x - ZmoraPos.x, TargetPos.y - ZmoraPos.y);
			if (dir.Length() > 1.f) {
				dir.Normalize();
				ZmoraPos.x += dir.x * m_Speed * DT * 10;
				ZmoraPos.y += dir.y * m_Speed * DT * 10;
			}
		}
	}
	else if (m_ZmoraState == ZMORA_STATE::ZMORA_TAKEDOWN) {
		float startY = TargetPos.y;
		int idx = m_CurObject->FlipbookRender()->GetSpriteIdx();
		//0~7 �ö󰡱� 8��
		//8~14 7��
		//�󸶸� �ö󰡰� ������ ���ΰ� 300���� ��ƺ���
		if (m_CurObject->FlipbookRender()->GetSpriteIdx() >= 0 && m_CurObject->FlipbookRender()->GetSpriteIdx() <= 8) {
			if (idx == 0) ZmoraPos.y = startY;
			float t = 1.f - (float)idx / 8.f;
			t = t * t; // t�� �����ؼ� ó���� �ſ� ������ ���� ���ӵ�
			float speed = t * m_Speed * 10.f;
			ZmoraPos.y += speed * DT;
		}
		else if (m_CurObject->FlipbookRender()->GetSpriteIdx() >= 8 && m_CurObject->FlipbookRender()->GetSpriteIdx() <= 14) {
			float t = 1.f - ((float)(idx - 8) / 6.f);
			t = t * t; // ���������� ������ �� �ѷ���
			float speed = t * m_Speed * 10.f;
			ZmoraPos.y -= speed * DT;
		}
	}

	else if (m_ZmoraState == ZMORA_STATE::ZMORA_WALK) {
		m_CoolTime += DT;
		m_TakeDownCoolTime += DT;

		if (ZmoraPos.x - TargetPos.x < 0 && !m_bBlockRight) {
			ZmoraPos.x += DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(1);
		}
		else if (!m_bBlockLeft) {
			ZmoraPos.x -= DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(0);
		}

		if (ZmoraPos.y - TargetPos.y < 0 && !m_bBlockUp) {
			ZmoraPos.y += DT * m_Speed;
		}
		else if (!m_bBlockRight) {
			ZmoraPos.y -= DT * m_Speed;
		}
	}


	m_CurObject->Transform()->SetRelativePos(ZmoraPos);

}

void CZmoraScript::ResetCoolTime()
{
	m_CoolTime = 0.f;
	m_TakeDownCoolTime = 0.f;
}

void CZmoraScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	//�� ������Ʈ�� ���
	if (OtherName == L"pWall") {
		Vec2 playerPos = this->Collider2D()->GetScale();
		Vec2 wallPos = _Other->Collider2D()->GetScale();

		Vec2 dir = playerPos - wallPos;

		if (abs(dir.x) > abs(dir.y)) {
			// X�� �浹
			if (dir.x > 0) {
				m_bBlockRight = true;   // ���� ���ʿ� ���� �� ���� �̵� ����
			}
			else {
				m_bBlockLeft = true;  // ���� �����ʿ� ���� �� ������ �̵� ����
			}
		}
		else {
			// Y�� �浹
			if (dir.y > 0) {
				m_bBlockUp = true;     // ���� ���ʿ� ���� �� ���� �̵� ����
			}
			else {
				m_bBlockDown = true;   // ���� �Ʒ��ʿ� ���� �� �Ʒ��� �̵� ����
			}
		}
	}
}

void CZmoraScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{

	if (_Other->GetName() == L"pBody" && m_ZmoraState == ZMORA_STATE::ZMORA_SWING) {
		CGameObject* m_CurObject = this->GetOwner();
		int idx = m_CurObject->FlipbookRender()->GetSpriteIdx();
		if (idx >= 4 && idx <= 5) {
			//PlayerHeat ���� ��� ������ �װ� ��� ������ ����� �μ�
			_Other->SetPlayerHeat(true);
		}
	}
	else if (_Other->GetName() == L"pBody" && m_ZmoraState == ZMORA_STATE::ZMORA_TAKEDOWN) {
		CGameObject* m_CurObject = this->GetOwner();
		int idx = m_CurObject->FlipbookRender()->GetSpriteIdx();
		if (idx >= 9 && idx <= 10) {
			//PlayerHeat ���� ��� ������ �װ� ��� ������ ����� �μ�
			_Other->SetPlayerHeat(true);
		}
	}
}

void CZmoraScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	if (OtherName == L"pWall") {
		m_bBlockLeft = false;
		m_bBlockRight = false;
		m_bBlockUp = false;
		m_bBlockDown = false;
	}
}

void CZmoraScript::SaveToFile(FILE* _File)
{
}

void CZmoraScript::LoadFromFile(FILE* _File)
{
}

