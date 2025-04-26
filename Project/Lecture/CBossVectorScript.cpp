#include "pch.h"
#include "CBossVectorScript.h"

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

#include "CBombSkulScript.h"
#include "CBigRhinoScript.h"
#include "CSkulScript.h"
#include "CSmallRhinoScript.h"

//랜덤 함수 사용
#include <random>

CBossVectorScript::CBossVectorScript()
	:CScript(BOSSVECTORSCRIPT)
	, CoolTime(0.f)
	, WaitTime(0.f)
	, SkulLeftRight(0)
	, SpawnCount(0)
	, SpawnMonsterType(0)
	, m_Vector_state(BOSS_VECTOR_STATE::BOSS_VECTOR_START)
{
}

CBossVectorScript::~CBossVectorScript()
{
}

void CBossVectorScript::Init()
{
}

void CBossVectorScript::Begin()
{
}

void CBossVectorScript::Tick()
{
	CoolTime += DT;

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pGhostLayer = pLevel->GetLayer(6); //GhostLayer = 6
	const vector<CGameObject*>& vecObj = pGhostLayer->GetParentObjects();

	if (FlipbookRender()->IsFinish() && m_Vector_state == BOSS_VECTOR_STATE::BOSS_VECTOR_START) {
		FlipbookRender()->Play(1, 9, false);
		m_Vector_state = BOSS_VECTOR_STATE::BOSS_VECTOR_SPAWN_RHINO;
		float y = Transform()->GetRelativePos().y;
		float x = Transform()->GetRelativePos().x;
		float z = Transform()->GetRelativePos().z;
		y += 200;

		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_summon_deer_big_01 [1].wav");
		pSound->Play(1, 0.2f, false);

		Ptr<CSound> pSound2 = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_reappear_01 [1].wav");
		pSound2->Play(1, 0.2f, false);


		Transform()->SetRelativePos(x, y, z);
		SpawnCount += 1;
		CGameObject* pBigRhino = new CGameObject;
		pBigRhino->SetName(L"BigRhino");
		pBigRhino->AddComponent(new CTransform);
		pBigRhino->AddComponent(new CFlipbookRender);
		pBigRhino->AddComponent(new CCollider2D);
		pBigRhino->AddComponent(new CBigRhinoScript);
		pBigRhino->Transform()->SetRelativePos(0.f, -200.f, -5.f);
		pBigRhino->Transform()->SetRelativeScale(328, 232.f, 1.f);
		pBigRhino->Collider2D()->SetOffset(Vec2(0.f, 0.f));
		pBigRhino->Collider2D()->SetScale(Vec2(228.1f, 137.1f));
		pBigRhino->Collider2D()->SetIndependentScale(true);
		pBigRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Spawn.flip"));
		pBigRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Walk.flip"));
		pBigRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Attack.flip"));
		pBigRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Death.flip"));
		pBigRhino->FlipbookRender()->Play(0, 8, false);
		SpawnGameObject(pBigRhino, 6);

	}
	else {
		//폭탄 0
		//코뿔소 1
		//해골 2
		if (CoolTime > 4.f) {
			if (SpawnCount < 2) {
				bool SpawnMonster = false;

				//현재 플립북이 끝났으면 늘림
				if (FlipbookRender()->IsFinish()) {
					WaitTime += DT;
				}
				//이전 Flipbook이 끝나고 1초 정도 지났을때 다음 Spawn 실행
				if (FlipbookRender()->IsFinish() && WaitTime > 1.f) {
					if (SpawnMonsterType == 0) {
						bool spawn = false;

						for (int i = 0; i < vecObj.size(); i++) {
							if (vecObj[i]->GetName() == L"BombSkul") {
								spawn = true;
								break;
							}
						}
						if (spawn == false) {
							Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_summon_skull_01 [1].wav");
							pSound->Play(1, 0.2f, false);
							//폭탄 소환
							CGameObject* pBombSkul = new CGameObject;
							pBombSkul->SetName(L"BombSkul");
							pBombSkul->AddComponent(new CTransform);
							pBombSkul->AddComponent(new CFlipbookRender);
							pBombSkul->AddComponent(new CCollider2D);
							pBombSkul->AddComponent(new CBombSkulScript);
							pBombSkul->Transform()->SetRelativePos(0.f, -200.f, -5.f);
							pBombSkul->Transform()->SetRelativeScale(140, 168.f, 1.f);
							pBombSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
							pBombSkul->Collider2D()->SetScale(Vec2(200.1f, 200.1f));
							pBombSkul->Collider2D()->SetIndependentScale(true);
							pBombSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Spawn.flip"));
							pBombSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Walk.flip"));
							pBombSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_ExplosionAttack.flip"));
							pBombSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Death.flip"));
							pBombSkul->FlipbookRender()->Play(0, 8, false);
							SpawnGameObject(pBombSkul, 6);

							FlipbookRender()->Play(BOSS_VECTOR_STATE::BOSS_VECTOR_SPAWN_BOMB, 9, false);
							SpawnMonster = true;
							SpawnCount += 1;
						}
						SpawnMonsterType += 1;

					}
					else if (SpawnMonsterType == 1) {
						bool spawn = false;

						for (int i = 0; i < vecObj.size(); i++) {
							if (vecObj[i]->GetName() == L"SmallRhino") {
								spawn = true;
								break;
							}
						}
						if (spawn == false) {
							//작은 코뿔소 소환
							Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_summon_deer_small_01 [1].wav");
							pSound->Play(1, 0.2f, false);
							FlipbookRender()->Play(BOSS_VECTOR_STATE::BOSS_VECTOR_SPAWN_RHINO, 9, false);

							CGameObject* pSmallRhino = new CGameObject;
							pSmallRhino->SetName(L"SmallRhino");
							pSmallRhino->AddComponent(new CTransform);
							pSmallRhino->AddComponent(new CFlipbookRender);
							pSmallRhino->AddComponent(new CCollider2D);
							pSmallRhino->AddComponent(new CSmallRhinoScript);
							pSmallRhino->Transform()->SetRelativePos(-397.f, -50.f, -5.f);
							pSmallRhino->Transform()->SetRelativeScale(120, 160.f, 1.f);
							pSmallRhino->Collider2D()->SetOffset(Vec2(0.f, -42.f));
							pSmallRhino->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
							pSmallRhino->Collider2D()->SetIndependentScale(true);
							pSmallRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Spawn.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Walk.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Attack.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
							pSmallRhino->FlipbookRender()->Play(0, 8, false);
							SpawnGameObject(pSmallRhino, 6);

							pSmallRhino = new CGameObject;
							pSmallRhino->SetName(L"SmallRhino");
							pSmallRhino->AddComponent(new CTransform);
							pSmallRhino->AddComponent(new CFlipbookRender);
							pSmallRhino->AddComponent(new CCollider2D);
							pSmallRhino->AddComponent(new CSmallRhinoScript);
							pSmallRhino->Transform()->SetRelativePos(-583.f, -252.f, -5.f);
							pSmallRhino->Transform()->SetRelativeScale(120, 160.f, 1.f);
							pSmallRhino->Collider2D()->SetOffset(Vec2(0.f, -42.f));
							pSmallRhino->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
							pSmallRhino->Collider2D()->SetIndependentScale(true);
							pSmallRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Spawn.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Walk.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Attack.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
							pSmallRhino->FlipbookRender()->Play(0, 8, false);
							SpawnGameObject(pSmallRhino, 6);

							pSmallRhino = new CGameObject;
							pSmallRhino->SetName(L"SmallRhino");
							pSmallRhino->AddComponent(new CTransform);
							pSmallRhino->AddComponent(new CFlipbookRender);
							pSmallRhino->AddComponent(new CCollider2D);
							pSmallRhino->AddComponent(new CSmallRhinoScript);
							pSmallRhino->Transform()->SetRelativePos(571.f, -252.f, -5.f);
							pSmallRhino->Transform()->SetRelativeScale(120, 160.f, 1.f);
							pSmallRhino->Collider2D()->SetOffset(Vec2(0.f, -42.f));
							pSmallRhino->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
							pSmallRhino->Collider2D()->SetIndependentScale(true);
							pSmallRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Spawn.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Walk.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Attack.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
							pSmallRhino->FlipbookRender()->Play(0, 8, false);
							SpawnGameObject(pSmallRhino, 6);

							pSmallRhino = new CGameObject;
							pSmallRhino->SetName(L"SmallRhino");
							pSmallRhino->AddComponent(new CTransform);
							pSmallRhino->AddComponent(new CFlipbookRender);
							pSmallRhino->AddComponent(new CCollider2D);
							pSmallRhino->AddComponent(new CSmallRhinoScript);
							pSmallRhino->Transform()->SetRelativePos(400.f, -50.f, -5.f);
							pSmallRhino->Transform()->SetRelativeScale(120, 160.f, 1.f);
							pSmallRhino->Collider2D()->SetOffset(Vec2(0.f, -42.f));
							pSmallRhino->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
							pSmallRhino->Collider2D()->SetIndependentScale(true);
							pSmallRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Spawn.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Walk.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Attack.flip"));
							pSmallRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
							pSmallRhino->FlipbookRender()->Play(0, 8, false);
							SpawnGameObject(pSmallRhino, 6);
							SpawnMonster = true;
							SpawnCount += 1;
						}
						SpawnMonsterType += 1;
					}
					else if (SpawnMonsterType == 2) {
						bool spawn = false;

						if (SkulLeftRight == 0) {
							for (int i = 0; i < vecObj.size(); i++) {
								if (vecObj[i]->GetName() == L"SkulLeft") {
									spawn = true;
									break;
								}
							}
						}
						else {
							for (int i = 0; i < vecObj.size(); i++) {
								if (vecObj[i]->GetName() == L"SkulRight") {
									spawn = true;
									break;
								}
							}
						}

						if (spawn == false) {
							//달리는 해골 소환
							Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_summon_skeletons_01 [1].wav");
							pSound->Play(1, 0.2f, false);
							FlipbookRender()->Play(BOSS_VECTOR_STATE::BOSS_VECTOR_SPAWN_SKUL, 9, false);
							SpawnCount += 1;
							//왼쪽으로 달리는 해골 소환
							if (SkulLeftRight == 0) {
								CGameObject* pSkul = new CGameObject;
								pSkul->SetName(L"SkulLeft");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(535.f, -25.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulLeft");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(545.f, -120.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulLeft");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(555.f, -220.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulLeft");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(565.f, -320.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								SpawnMonster = true;
							}
							//오른쪽으로 달리는 해골 소환
							else {
								Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_boss_hum_summon_skeletons_01 [1].wav");
								pSound->Play(1, 0.2f, false);

								CGameObject* pSkul = new CGameObject;
								pSkul->SetName(L"SkulRight");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(-620.f, -25.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->SetDir(1);
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulRight");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(-630.f, -120.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->SetDir(1);
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulRight");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(-640.f, -220.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->SetDir(1);
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								pSkul = new CGameObject;
								pSkul->SetName(L"SkulRight");
								pSkul->AddComponent(new CTransform);
								pSkul->AddComponent(new CFlipbookRender);
								pSkul->AddComponent(new CCollider2D);
								pSkul->AddComponent(new CSkulScript);
								pSkul->Transform()->SetRelativePos(-650.f, -320.f, -5.f);
								pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
								pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
								pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
								pSkul->Collider2D()->SetIndependentScale(true);
								pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
								pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
								pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
								pSkul->FlipbookRender()->SetDir(1);
								pSkul->FlipbookRender()->Play(0, 8, true);
								SpawnGameObject(pSkul, 6);

								SpawnMonster = true;
							}
							SkulLeftRight += 1;
							SkulLeftRight = SkulLeftRight % 2;
						}

						SpawnMonsterType += 1;
					}

					SpawnMonsterType = SpawnMonsterType % 3;

				}
				if (SpawnMonster) {
					WaitTime = 0.f;
				}
				//2개 소환하면 CoolTime돔
				if (SpawnCount == 2) {
					SpawnCount = 0;
					CoolTime = 0.f;
				}
			}
		}
	}


}

void CBossVectorScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CBossVectorScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CBossVectorScript::SaveToFile(FILE* _File)
{
}

void CBossVectorScript::LoadFromFile(FILE* _File)
{
}
