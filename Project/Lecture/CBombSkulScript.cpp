#include "pch.h"
#include "CBombSkulScript.h"

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


CBombSkulScript::CBombSkulScript()
	:CScript(BOMBSKULSCRIPT)
	, TargetPos(0.f, 0.f, 0.f)
	, m_Bomb_State(BOMBSKUL_STATE::BOMBSKUL_SPAWN)
	, m_Speed(10.f)
{
}

CBombSkulScript::~CBombSkulScript()
{
}

void CBombSkulScript::Init()
{
}

void CBombSkulScript::Begin()
{
}

void CBombSkulScript::Tick()
{
	m_Speed = 20.f;
	TargetPos = Vec3(0.f, 0.f, 0.f);

	CGameObject* m_CurObject = this->GetOwner();
	Vec3 BombPos = m_CurObject->Transform()->GetRelativePos();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(5); //PlayerLayer = 5
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	if (m_Bomb_State == BIG_RHINO_STATE::BIG_RHINO_WALK) {
		for (int j = 0; j < vecObj.size(); j++) {
			if (vecObj[j]->GetName() == L"pBody") {
				TargetPos = vecObj[j]->Transform()->GetWorldPos();
			}
		}
	}

	if (m_Bomb_State == BOMBSKUL_STATE::BOMBSKUL_SPAWN && m_CurObject->FlipbookRender()->IsFinish()) {
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_skull_spawn_01 [1].wav");
		pSound->Play(1, 0.2f, false);

		m_Bomb_State = BOMBSKUL_STATE::BOMBSKUL_WALK;
		m_CurObject->FlipbookRender()->Play(BOMBSKUL_STATE::BOMBSKUL_WALK, 8, true);
	}
	else if (m_Bomb_State == BOMBSKUL_STATE::BOMBSKUL_ATTACK && m_CurObject->FlipbookRender()->IsFinish()) {
		m_CurObject->Destroy(); //시발 왜 삭제하면 오류나는건지 이해가안되네
		CGameObject* Scrap;

		float angle = 0;

		//22.5 각도로
		for (int i = 0; i < 16; i++) {

			Scrap = new CGameObject;
			Scrap->SetName(L"Scrap");
			Scrap->AddComponent(new CTransform);
			Scrap->AddComponent(new CCollider2D);
			Scrap->AddComponent(new CMeshRender);
			Scrap->AddComponent(new CMissileScript);
			Scrap->SetShoatAngle(angle);
			Scrap->Transform()->SetRelativePos(BombPos);
			Scrap->Transform()->SetRelativeScale(36.f, 42.f, 1.f);
			Scrap->Collider2D()->SetOffset(Vec2(0.f, 0.f));
			Scrap->Collider2D()->SetScale(Vec2(30.1f, 36.1f));
			Scrap->Collider2D()->SetIndependentScale(true);
			Scrap->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

			int idx = i % 10;
			Ptr<CMaterial> pMtrl;
			switch (idx)
			{
			case 0:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap0"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0001.png"));
				break;
			case 1:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap1"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0002.png"));
				break;
			case 2:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap2"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0003.png"));
				break;
			case 3:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap3"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0004.png"));
				break;
			case 4:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap4"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0005.png"));
				break;
			case 5:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap5"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0006.png"));
				break;
			case 6:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap6"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0007.png"));
				break;
			case 7:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap7"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0008.png"));
				break;
			case 8:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap8"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0009.png"));
				break;
			case 9:
				Scrap->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Scrap9"));
				pMtrl = Scrap->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\scrapMetal0010.png"));
				break;
			default:
				break;
			}

			pMtrl->SetScalarParam(INT_0, 0);
			SpawnGameObject(Scrap, 6);

			angle += 22.5;

		}
	}

	if (m_Bomb_State == BOMBSKUL_STATE::BOMBSKUL_WALK) {
		if (BombPos.x - TargetPos.x < 0) {
			BombPos.x += DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(1);
		}
		else {
			BombPos.x -= DT * m_Speed;
			m_CurObject->FlipbookRender()->SetDir(0);
		}

		if (BombPos.y - TargetPos.y < 0) {
			BombPos.y += DT * m_Speed;
		}
		else {
			BombPos.y -= DT * m_Speed;
		}
	}

	m_CurObject->Transform()->SetRelativePos(BombPos);

}

void CBombSkulScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	if (OtherName == L"pBody") {
		m_Bomb_State = BOMBSKUL_STATE::BOMBSKUL_ATTACK;
		CGameObject* m_CurObject = this->GetOwner();
		m_CurObject->FlipbookRender()->Play(BOMBSKUL_STATE::BOMBSKUL_ATTACK, 8, false);
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_skull_death_01 [1].wav");
		pSound->Play(1, 0.2f, false);
	}
}

void CBombSkulScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CBombSkulScript::SaveToFile(FILE* _File)
{
}

void CBombSkulScript::LoadFromFile(FILE* _File)
{
}


