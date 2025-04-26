#include "pch.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CTaskMgr.h>

#include <Lecture/Scripts/CPlayerScript.h>
#include <Lecture/Scripts/CMissileScript.h>
#include <Lecture/Scripts/CCameraMoveScript.h>
#include <Lecture/Scripts/CGeneratorScript.h>
#include <Lecture/Scripts/CAparatusScript.h>
#include <Lecture/Scripts/CZmoraScript.h>
#include <Lecture/Scripts/CBigRhinoScript.h>
#include <Lecture/Scripts/CSmallRhinoScript.h>
#include <Lecture/Scripts/CSkulScript.h>
#include <Lecture/Scripts/CBombSkulScript.h>
#include <Lecture/Scripts/CBossVectorScript.h>

//#include <Lecture/Scripts/CGeneratorScript.h>

void CreateTestLevel()
{

	//Defact 보스 BGM
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	pCurLevel->SetName(LEVEL_LOBBY);

	//pCurLevel->GetLayer(0)->SetName(L"Default");
	//pCurLevel->GetLayer(1)->SetName(L"Background");
	//pCurLevel->GetLayer(2)->SetName(L"Tile");
	//pCurLevel->GetLayer(3)->SetName(L"Player");
	//pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	//pCurLevel->GetLayer(5)->SetName(L"Enermy");
	//pCurLevel->GetLayer(6)->SetName(L"EnermyProjectile");

	for (int i = 0; i < COLLISION_LAYER::END; i++) {
		pCurLevel->GetLayer(i)->SetName(COLLISION_LAYER_WSTR[(UINT)i]);
	}

	// 카메라 역할 오브젝트 생성
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(0.f, 0.f, -100.f);

	//pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pCamObj->Camera()->SetPriority(0); // 메인 카메라로 설정
	pCamObj->Camera()->CheckLayerAll();
	pCamObj->Camera()->CheckLayer(31);

	SpawnGameObject(pCamObj, 0);

	// 광원 설치
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light");

	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Transform()->SetRelativePos(-200.f, 0.f, 0.f);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
	pLight->Light2D()->SetRadius(300.f);

	SpawnGameObject(pLight, 0);

	//CGameObject* pGlassFloor = new CGameObject;
	//pGlassFloor->SetName(L"pGlassFloor");
	//pGlassFloor->AddComponent(new CTransform);
	//pGlassFloor->AddComponent(new CFlipbookRender);
	//pGlassFloor->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pGlassFloor->Transform()->SetRelativeScale(100, 100.f, 1.f);
	//pGlassFloor->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\GlassFloor.flip"));
	//pGlassFloor->FlipbookRender()->Play(0, 10, true);
	//SpawnGameObject(pGlassFloor, 7);

	// Body Object
	CGameObject* pBody = new CGameObject;
	pBody->SetName(L"pBody");
	pBody->AddComponent(new CTransform);
	//pBody->AddComponent(new CMeshRender);
	pBody->AddComponent(new CCollider2D);
	pBody->AddComponent(new CFlipbookRender);
	pBody->AddComponent(new CPlayerScript);

	pBody->Transform()->SetRelativePos(0.f, -30.f, -50.f);
	pBody->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	//pBody->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pBody->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	//pBody->Collider2D()->SetIndependentScale(true);
	pBody->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pBody->Collider2D()->SetScale(Vec2(100.f, 100.f));
	pBody->Collider2D()->SetIndependentScale(true);
	pBody->Transform()->SetIndependentScale(true);

	pBody->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CharaIdle.flip"));
	pBody->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Chara_Run.flip"));
	pBody->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CharaDash.flip"));

	pBody->FlipbookRender()->Play(0, 10, true);
	//부모 자식 연결

	// Head Object
	CGameObject* pHead = new CGameObject;
	pHead->SetName(L"pHead");
	pHead->AddComponent(new CTransform);
	pHead->AddComponent(new CMeshRender);
	pHead->AddComponent(new CCollider2D);
	pHead->AddComponent(new CPlayerScript);
	pHead->Transform()->SetRelativePos(0.f, 40.f, -13.f);
	pHead->Transform()->SetRelativeScale(75.f, 75.f, 1.f);
	pHead->Transform()->SetIndependentScale(true);
	//pHead->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pHead->Collider2D()->SetScale(Vec2(100.1f, 100.1f));
	//pHead->Collider2D()->SetIndependentScale(true);

	pHead->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pHead->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PlayerHead"));

	Ptr<CMaterial> pMtrl = pHead->MeshRender()->GetMaterial();
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaHead0002.png"));
	pMtrl->SetScalarParam(INT_0, 0);
	pBody->AddChild(pHead);

	//CGameObject* pHead = new CGameObject;
	//pHead->SetName(L"pHead");
	//pHead->AddComponent(new CTransform);
	////pHead->AddComponent(new CMeshRender);
	//pHead->AddComponent(new CCollider2D);
	//pHead->AddComponent(new CFlipbookRender);
	//pHead->AddComponent(new CPlayerScript);
	//pHead->Transform()->SetRelativePos(0.f, 38.f, -13.f);
	//pHead->Transform()->SetRelativeScale(75.f, 75.f, 1.f);
	//pHead->Transform()->SetIndependentScale(true);
	//pHead->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pHead->Collider2D()->SetScale(Vec2(75.1f, 75.1f));
	//pHead->Collider2D()->SetIndependentScale(true);
	//pHead->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Chara_Head.flip"));
	//pHead->FlipbookRender()->Play(0, 10, true);
	//pBody->AddChild(pHead);
	//Chara_Head

	CGameObject* pLeftHand = new CGameObject;
	pLeftHand->SetName(L"pLeftHand");
	pLeftHand->AddComponent(new CTransform);
	pLeftHand->AddComponent(new CMeshRender);
	pLeftHand->AddComponent(new CCollider2D);
	pLeftHand->AddComponent(new CPlayerScript);
	pLeftHand->Transform()->SetRelativePos(20.f, -6.f, -11.f);
	pLeftHand->Transform()->SetRelativeScale(40.f, 40.f, 1.f);
	pLeftHand->Transform()->SetIndependentScale(true);

	pLeftHand->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pLeftHand->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PlayerLeftHand"));

	pMtrl = pLeftHand->MeshRender()->GetMaterial();
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaHand0008.png"));
	pMtrl->SetScalarParam(INT_0, 0);
	pBody->AddChild(pLeftHand);

	CGameObject* pRightHand = new CGameObject;
	pRightHand->SetName(L"pRightHand");
	pRightHand->AddComponent(new CTransform);
	pRightHand->AddComponent(new CMeshRender);
	pRightHand->AddComponent(new CCollider2D);
	pRightHand->AddComponent(new CPlayerScript);
	pRightHand->Transform()->SetRelativePos(-20.f, -8.f, -9.f);
	pRightHand->Transform()->SetRelativeScale(40.f, 40.f, 1.f);
	pRightHand->Transform()->SetIndependentScale(true);

	pRightHand->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pRightHand->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PlayerRightHand"));

	pMtrl = pRightHand->MeshRender()->GetMaterial();
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaHand0001.png"));
	pMtrl->SetScalarParam(INT_0, 0);
	pBody->AddChild(pRightHand);

	//PlayerObject 소환
	SpawnGameObject(pBody, 5);

	//CGameObject* pBombSkul = new CGameObject;
	//pBombSkul->SetName(L"BombSkul");
	//pBombSkul->AddComponent(new CTransform);
	//pBombSkul->AddComponent(new CFlipbookRender);
	//pBombSkul->AddComponent(new CCollider2D);
	//pBombSkul->AddComponent(new CBombSkulScript);
	//pBombSkul->Transform()->SetRelativePos(0.f, -200.f, 0.f);
	//pBombSkul->Transform()->SetRelativeScale(140, 168.f, 1.f);
	//pBombSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pBombSkul->Collider2D()->SetScale(Vec2(200.1f, 200.1f));
	//pBombSkul->Collider2D()->SetIndependentScale(true);
	//pBombSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Spawn.flip"));
	//pBombSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Walk.flip"));
	//pBombSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_ExplosionAttack.flip"));
	//pBombSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\BombSkul_Death.flip"));
	//pBombSkul->FlipbookRender()->Play(0, 8, false);
	//SpawnGameObject(pBombSkul, 6);

	//CGameObject* pBigRhino = new CGameObject;
	//pBigRhino->SetName(L"BigRhino");
	//pBigRhino->AddComponent(new CTransform);
	//pBigRhino->AddComponent(new CFlipbookRender);
	//pBigRhino->AddComponent(new CCollider2D);
	//pBigRhino->AddComponent(new CBigRhinoScript);
	//pBigRhino->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pBigRhino->Transform()->SetRelativeScale(328, 232.f, 1.f);
	//pBigRhino->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pBigRhino->Collider2D()->SetScale(Vec2(228.1f, 137.1f));
	//pBigRhino->Collider2D()->SetIndependentScale(true);
	//pBigRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Spawn.flip"));
	//pBigRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Walk.flip"));
	//pBigRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Attack.flip"));
	//pBigRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Big_Rhino_Death.flip"));
	//pBigRhino->FlipbookRender()->Play(0, 8, false);
	//SpawnGameObject(pBigRhino, 6);

	//CGameObject* pSmallRhino = new CGameObject;
	//pSmallRhino->SetName(L"SmallRhino");
	//pSmallRhino->AddComponent(new CTransform);
	//pSmallRhino->AddComponent(new CFlipbookRender);
	//pSmallRhino->AddComponent(new CCollider2D);
	//pSmallRhino->AddComponent(new CSmallRhinoScript);
	//pSmallRhino->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pSmallRhino->Transform()->SetRelativeScale(120, 160.f, 1.f);
	//pSmallRhino->Collider2D()->SetOffset(Vec2(0.f, -42.f));
	//pSmallRhino->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
	//pSmallRhino->Collider2D()->SetIndependentScale(true);
	//pSmallRhino->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Spawn.flip"));
	//pSmallRhino->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Walk.flip"));
	//pSmallRhino->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Attack.flip"));
	//pSmallRhino->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
	//pSmallRhino->FlipbookRender()->Play(0, 8, false);
	//SpawnGameObject(pSmallRhino, 6);

	//CGameObject* pSkul = new CGameObject;
	//pSkul->SetName(L"SkulRight");
	//pSkul->AddComponent(new CTransform);
	//pSkul->AddComponent(new CFlipbookRender);
	//pSkul->AddComponent(new CCollider2D);
	//pSkul->AddComponent(new CSkulScript);
	//pSkul->Transform()->SetRelativePos(0.f, -200.f, 0.f);
	//pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
	//pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
	//pSkul->Collider2D()->SetIndependentScale(true);
	//pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
	//pSkul->FlipbookRender()->SetDir(1);
	//pSkul->FlipbookRender()->Play(0, 8, true);
	//SpawnGameObject(pSkul, 6);

	//pSkul = new CGameObject;
	//pSkul->SetName(L"SkulLeft");
	//pSkul->AddComponent(new CTransform);
	//pSkul->AddComponent(new CFlipbookRender);
	//pSkul->AddComponent(new CCollider2D);
	//pSkul->AddComponent(new CSkulScript);
	//pSkul->Transform()->SetRelativePos(0.f, -200.f, 0.f);
	//pSkul->Transform()->SetRelativeScale(120, 160.f, 1.f);
	//pSkul->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pSkul->Collider2D()->SetScale(Vec2(81.1f, 82.1f));
	//pSkul->Collider2D()->SetIndependentScale(true);
	//pSkul->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Spawn.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ready_Run.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Skul_Ing_Run.flip"));
	//pSkul->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Small_Rhino_Death.flip"));
	//pSkul->FlipbookRender()->Play(0, 8, true);
	//SpawnGameObject(pSkul, 6);

	//CGameObject* pBossVector = new CGameObject;
	//pBossVector->SetName(L"BossVector");
	//pBossVector->AddComponent(new CTransform);
	//pBossVector->AddComponent(new CFlipbookRender);
	//pBossVector->AddComponent(new CBossVectorScript);
	//pBossVector->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	//pBossVector->Transform()->SetRelativeScale(1028, 868.f, 1.f);
	//pBossVector->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Boss_Vector_Start.flip"));
	//pBossVector->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Boss_Spawn_Rhino.flip"));
	//pBossVector->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Boss_Spawn_Skul.flip"));
	//pBossVector->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Boss_Spawn_Bomb.flip"));
	//pBossVector->FlipbookRender()->Play(0, 15, false);
	//SpawnGameObject(pBossVector, 6);

	CGameObject* pGear = new CGameObject;
	pGear->SetName(L"Gear_Aparatus");
	pGear->AddComponent(new CTransform);
	pGear->AddComponent(new CFlipbookRender);
	pGear->AddComponent(new CCollider2D);
	pGear->AddComponent(new CAparatusScript);
	pGear->Transform()->SetRelativePos(300.f, 0.f, 0.f);
	pGear->Transform()->SetRelativeScale(200, 200.f, 1.f);
	pGear->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pGear->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pGear->Collider2D()->SetIndependentScale(true);
	pGear->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\GearEndFlipbook.flip"));
	pGear->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\GearIngFlipbook.flip"));
	pGear->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\GearStartFlipbook.flip"));
	pGear->FlipbookRender()->Play(0, 10, false);
	SpawnGameObject(pGear, 4);

	CGameObject* pCopper = new CGameObject;
	pCopper->SetName(L"Copper_Aparatus");
	pCopper->AddComponent(new CTransform);
	pCopper->AddComponent(new CFlipbookRender);
	pCopper->AddComponent(new CCollider2D);
	pCopper->AddComponent(new CAparatusScript);
	pCopper->Transform()->SetRelativePos(300.f, -200.f, 0.f);
	pCopper->Transform()->SetRelativeScale(200, 200.f, 1.f);
	pCopper->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pCopper->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pCopper->Collider2D()->SetIndependentScale(true);
	pCopper->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CopperEndFlipbook.flip"));
	pCopper->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CopperIngFlipbook.flip"));
	pCopper->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CopperStartFlipbook.flip"));
	pCopper->FlipbookRender()->Play(0, 10, false);
	SpawnGameObject(pCopper, 4);

	CGameObject* pMachined_Copper = new CGameObject;
	pMachined_Copper->SetName(L"Machined_Copper_Aparatus");
	pMachined_Copper->AddComponent(new CTransform);
	pMachined_Copper->AddComponent(new CFlipbookRender);
	pMachined_Copper->AddComponent(new CCollider2D);
	pMachined_Copper->AddComponent(new CAparatusScript);
	pMachined_Copper->Transform()->SetRelativePos(50.f, -200.f, 0.f);
	pMachined_Copper->Transform()->SetRelativeScale(200, 200.f, 1.f);
	pMachined_Copper->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pMachined_Copper->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pMachined_Copper->Collider2D()->SetIndependentScale(true);
	pMachined_Copper->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Machined_CopperEndFlipbook.flip"));
	pMachined_Copper->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Machined_CopperIngFlipbook.flip"));
	pMachined_Copper->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Machined_CopperStartFlipbook.flip"));
	pMachined_Copper->FlipbookRender()->Play(0, 10, false);
	SpawnGameObject(pMachined_Copper, 4);

	CGameObject* pMachined_Carbon = new CGameObject;
	pMachined_Carbon->SetName(L"Machined_Carbon_Aparatus");
	pMachined_Carbon->AddComponent(new CTransform);
	pMachined_Carbon->AddComponent(new CFlipbookRender);
	pMachined_Carbon->AddComponent(new CCollider2D);
	pMachined_Carbon->AddComponent(new CAparatusScript);
	pMachined_Carbon->Transform()->SetRelativePos(-150.f, -200.f, 0.f);
	pMachined_Carbon->Transform()->SetRelativeScale(200, 200.f, 1.f);
	pMachined_Carbon->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pMachined_Carbon->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pMachined_Carbon->Collider2D()->SetIndependentScale(true);
	pMachined_Carbon->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Pro_Aparatus_End.flip"));
	pMachined_Carbon->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Pro_Aparatus_Ing.flip"));
	pMachined_Carbon->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Pro_Aparatus_Start.flip"));
	pMachined_Carbon->FlipbookRender()->Play(0, 10, false);
	SpawnGameObject(pMachined_Carbon, 4);

	CGameObject* Carbon = new CGameObject;
	Carbon->SetName(L"Carbon_Aparatus");
	Carbon->AddComponent(new CTransform);
	Carbon->AddComponent(new CFlipbookRender);
	Carbon->AddComponent(new CCollider2D);
	Carbon->AddComponent(new CAparatusScript);
	Carbon->Transform()->SetRelativePos(-350.f, -200.f, 0.f);
	Carbon->Transform()->SetRelativeScale(200, 200.f, 1.f);
	Carbon->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	Carbon->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	Carbon->Collider2D()->SetIndependentScale(true);
	Carbon->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CarbonStartFlipbook.flip"));
	Carbon->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CarbonIngFlipbook.flip"));
	Carbon->FlipbookRender()->Play(0, 10, false);
	SpawnGameObject(Carbon, 4);

	CGameObject* pGenerator = new CGameObject;
	pGenerator->SetName(L"Generator_0");
	pGenerator->AddComponent(new CTransform);
	pGenerator->AddComponent(new CFlipbookRender);
	pGenerator->AddComponent(new CCollider2D);
	pGenerator->AddComponent(new CGeneratorScript);
	pGenerator->Transform()->SetRelativePos(300.f, 400.f, 0.f);
	pGenerator->Transform()->SetRelativeScale(200, 200.f, 1.f);

	pGenerator->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pGenerator->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pGenerator->Collider2D()->SetIndependentScale(true);

	pGenerator->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Start.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Ing.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Start.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Ing.flip"));
	pGenerator->FlipbookRender()->Play(0, 10, true);
	SpawnGameObject(pGenerator, 4);

	//pGenerator = new CGameObject;
	//pGenerator->SetName(L"Generator_1");
	//pGenerator->AddComponent(new CTransform);
	//pGenerator->AddComponent(new CFlipbookRender);
	//pGenerator->AddComponent(new CCollider2D);
	//pGenerator->AddComponent(new CGeneratorScript);
	//pGenerator->Transform()->SetRelativePos(-100.f, 200.f, 0.f);
	//pGenerator->Transform()->SetRelativeScale(200, 200.f, 1.f);
	//pGenerator->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pGenerator->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	//pGenerator->Collider2D()->SetIndependentScale(true);

	//pGenerator->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Ing.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Ing.flip"));
	//pGenerator->FlipbookRender()->SetDir(1);
	//pGenerator->FlipbookRender()->Play(0, 10, true);
	//SpawnGameObject(pGenerator, 4);

	pGenerator = new CGameObject;
	pGenerator->SetName(L"Generator_2");
	pGenerator->AddComponent(new CTransform);
	pGenerator->AddComponent(new CFlipbookRender);
	pGenerator->AddComponent(new CCollider2D);
	pGenerator->AddComponent(new CGeneratorScript);
	pGenerator->Transform()->SetRelativePos(100.f, 200.f, 0.f);
	pGenerator->Transform()->SetRelativeScale(200, 200.f, 1.f);

	pGenerator->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pGenerator->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	pGenerator->Collider2D()->SetIndependentScale(true);

	pGenerator->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Start.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Ing.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Start.flip"));
	pGenerator->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Ing.flip"));
	pGenerator->FlipbookRender()->Play(0, 10, true);
	SpawnGameObject(pGenerator, 4);

	//pGenerator = new CGameObject;
	//pGenerator->SetName(L"Generator_3");
	//pGenerator->AddComponent(new CTransform);
	//pGenerator->AddComponent(new CFlipbookRender);
	//pGenerator->AddComponent(new CCollider2D);
	//pGenerator->AddComponent(new CGeneratorScript);
	//pGenerator->Transform()->SetRelativePos(-600.f, 200.f, 0.f);
	//pGenerator->Transform()->SetRelativeScale(200, 200.f, 1.f);

	//pGenerator->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pGenerator->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	//pGenerator->Collider2D()->SetIndependentScale(true);

	//pGenerator->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Ing.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Ing.flip"));
	//pGenerator->FlipbookRender()->Play(0, 10, true);
	//SpawnGameObject(pGenerator, 4);

	//CGameObject* pZmora = new CGameObject;
	//pZmora->SetName(L"Zmora");
	//pZmora->AddComponent(new CTransform);
	//pZmora->AddComponent(new CFlipbookRender);
	//pZmora->AddComponent(new CCollider2D);
	//pZmora->AddComponent(new CZmoraScript);
	//pZmora->Transform()->SetRelativePos(0.f, 200.f, -8.f);
	//pZmora->Transform()->SetRelativeScale(250, 250.f, 1.f);

	//pZmora->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pZmora->Collider2D()->SetScale(Vec2(150.1f, 150.1f));
	//pZmora->Collider2D()->SetIndependentScale(true);

	//pZmora->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Zmora_Spawn.flip"));
	//pZmora->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\ZmoraWalk.flip"));
	//pZmora->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\ZmoraSwing.flip"));
	//pZmora->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\ZmoraTakeDown.flip"));
	//pZmora->FlipbookRender()->Play(0, 10, true);
	//SpawnGameObject(pZmora, 6);

	CGameObject* pWall = new CGameObject;
	pWall->SetName(L"pWall");
	pWall->AddComponent(new CTransform);
	pWall->AddComponent(new CMeshRender);
	pWall->AddComponent(new CCollider2D);
	pWall->Transform()->SetRelativePos(-800.f, 0.f, 0.f);
	pWall->Transform()->SetRelativeScale(100.f, 500.f, 1.f);
	pWall->Transform()->SetIndependentScale(true);
	//pWall->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pWall->Collider2D()->SetScale(Vec2(100.1f, 1000.1f));
	pWall->Collider2D()->SetIndependentScale(true);

	pWall->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pWall->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));

	Ptr<CMaterial> pMtrlWall = pWall->MeshRender()->GetMaterial();
	pMtrlWall->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
	pMtrlWall->SetScalarParam(INT_0, 0);
	SpawnGameObject(pWall, 7);

	//pWall = new CGameObject;
	//pWall->SetName(L"pWall");
	//pWall->AddComponent(new CTransform);
	//pWall->AddComponent(new CMeshRender);
	//pWall->AddComponent(new CCollider2D);
	//pWall->Transform()->SetRelativePos(800.f, 0.f, 0.f);
	//pWall->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	//pWall->Transform()->SetIndependentScale(true);
	////pWall->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pWall->Collider2D()->SetScale(Vec2(100.1f, 1000.1f));
	//pWall->Collider2D()->SetIndependentScale(true);

	//pWall->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pWall->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));

	//pMtrlWall = pWall->MeshRender()->GetMaterial();
	//pMtrlWall->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
	//pMtrlWall->SetScalarParam(INT_0, 0);
	//SpawnGameObject(pWall, 7);

	//pGenerator = new CGameObject;
	//pGenerator->SetName(L"Generator");
	//pGenerator->AddComponent(new CTransform);
	//pGenerator->AddComponent(new CFlipbookRender);
	//pGenerator->AddComponent(new CCollider2D);
	//pGenerator->AddComponent(new CGeneratorScript);
	//pGenerator->SetID(2);
	//pGenerator->Transform()->SetRelativePos(0.f, 200.f, 0.f);
	//pGenerator->Transform()->SetRelativeScale(200, 200.f, 1.f);

	//pGenerator->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	//pGenerator->Collider2D()->SetScale(Vec2(180.1f, 180.1f));
	//pGenerator->Collider2D()->SetIndependentScale(true);

	//pGenerator->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Broken_Ing.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(2, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Start.flip"));
	//pGenerator->FlipbookRender()->AddFlipbook(3, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Fix_Ing.flip"));
	//pGenerator->FlipbookRender()->Play(0, 10, true);
	//SpawnGameObject(pGenerator, 4);

	// ========
	// Particle
	// ========
	//CGameObject* pObject = new CGameObject;
	//pObject->SetName(L"Particle");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CParticleRender);

	//pObject->Transform()->SetRelativePos(-500.f, 300.f, 50.f);
	//pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	//CParticleRender* pRender = pObject->ParticleRender();

	//// 파티클 용 텍스쳐 지정
	//Ptr<CTexture> pParticleTex = CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\particle\\ray.png");
	//pRender->SetParticleTex(pParticleTex);

	//pRender->SetSpawnRate(20.f);
	//pRender->SetSpawnShape(0);
	//pRender->SetSpawnShapeScale(Vec3(500.f, 500.f, 500.f));
	//pRender->SetMinLife(1.f);
	//pRender->SetMaxLife(3.f);
	//pRender->SetMinScale(Vec3(100.f, 30.f, 10.f));
	//pRender->SetMaxScale(Vec3(200.f, 60.f, 20.f));

	//pRender->SetSpaceType(0);

	//// AddVelocity 기능 On
	//pRender->SetModlue(PARTICLE_MODULE::ADD_VELOCITY, true);
	//pRender->SetAddVelocityType(3);
	//pRender->SetMinSpeed(100.f);
	//pRender->SetMaxSpeed(500.f);
	//pRender->SetFixedVelocity(Vec3(0.f, -1.f, 0.f));

	//// SpawnBurst 기능
	//pRender->SetModlue(PARTICLE_MODULE::SPAWN_BURST, true);
	//pRender->SetBurstParticleCount(100);
	//pRender->SetBurstRepeatCount(10);
	//pRender->SetBurstTerm(2.f);

	//// Scale 
	//pRender->SetModlue(PARTICLE_MODULE::SCALE, true);
	//pRender->SetStartScale(2.f);
	//pRender->SetEndScale(2.f);

	////Drag
	//pRender->SetModlue(PARTICLE_MODULE::DRAG, true);
	//pRender->SetDragDestNormalizeAge(0.6f);  // 수명 끝
	//pRender->SetDragLimitSpeed(100.f);		// 속력 0

	////NoiseForce
	//pRender->SetModlue(PARTICLE_MODULE::NOISE_FORCE, true);
	//pRender->SetNoiseForceTerm(0.2f);
	//pRender->SetNoiseForceScale(300.f);

	//// Render
	//pRender->SetModlue(PARTICLE_MODULE::RENDER, true);
	//pRender->SetStartColor(Vec4(1.f, 0.f, 0.f, 1.f));
	//pRender->SetEndColor(Vec4(0.f, 0.f, 1.f, 1.f));

	//pRender->SetFadeOut(true);
	//pRender->SetFadOutStartRatio(0.8f);

	//pRender->SetVelocityAlignment(true, true);

	// 오브젝트를 0번 레이어에 추가
	//SpawnGameObject(pObject, 0);


	// TileRender Object
	//pObject = new CGameObject;
	//pObject->SetName(L"TileRender");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CTileRender);

	//pObject->Transform()->SetRelativePos(-200.f, 300.f, 500.f);
	//pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

	//pObject->TileRender()->SetColRow(10, 10);
	//pObject->TileRender()->SetTileSize(Vec2(64.f, 64.f));

	//for (int i = 0; i < 10; ++i)
	//{
	//	for (int j = 0; j < 10; ++j)
	//	{
	//		pObject->TileRender()->SetSprite(i, j, CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\TileSprite8_0.sprite"));
	//	}
	//}
	//pObject->TileRender()->SetSprite(1, 0, CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floorbotTilesmap1_2_0.sprite"));
	//pObject->TileRender()->SetSprite(0, 2, CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floorbotTilesmap1_1_0.sprite"));
	//pObject->TileRender()->SetSprite(0, 3, CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floorbotTilesmap1_5_0.sprite"));

	//SpawnGameObject(pObject, 0);

	// 충돌 레이어 지정
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 4);
	CCollisionMgr::GetInst()->CollisionLayerCheck(4, 5);
	CCollisionMgr::GetInst()->CollisionLayerCheck(5, 6);
	CCollisionMgr::GetInst()->CollisionLayerCheck(5, 7);
	CCollisionMgr::GetInst()->CollisionLayerCheck(6, 7);
	//CCollisionMgr::GetInst()->CollisionLayerCheck(3, 6);
}
