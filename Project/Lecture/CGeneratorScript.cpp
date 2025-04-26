#include "pch.h"
#include "CGeneratorScript.h"

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
#include <Engine/CLayer.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbookRender.h>

#include <Engine/CLight2D.h>
#include <Engine/CCamera.h>
#include <Engine/CSpriteRender.h>
#include <Engine/CTileRender.h>
#include <Engine/CParticleRender.h>
#include <Engine/CSound.h>

//랜덤 함수 사용
#include <random>

CGeneratorScript::CGeneratorScript()
	:CScript(GENERATORSCRIPT)
	, Part_Come1(PART::NONE_HAND)
	, Part_Come2(PART::NONE_HAND)
	, Part_Want1(PART::NONE_HAND)
	, Part_Want2(PART::NONE_HAND)
	, pGenerator_Effect(nullptr)
	, FixVFX(nullptr)
	, FixAnima(nullptr)
	, pGenerator_Effect_spawn(false)
	, pGenerator_Effect_CoolTime(0.f)
	, ScriptRunningTime(1.f)
	, m_Generator_state(GENERATOR_STATE::GENERATOR_BREAK_START)
	, m_ReneratorUI_state(GENERATOR_UI_STATE::GENERATOR_UI_NONE)
	, victoryScreen_Time(0.f)
	, SpawnvictoryScreen(false)
	, pSound(nullptr)
{
}

CGeneratorScript::~CGeneratorScript()
{
}

void CGeneratorScript::Init()
{

}

void CGeneratorScript::Begin()
{
	CLevel* Cur_Level = CLevelMgr::GetInst()->GetCurrentLevel();
	//Cur_Level->ChangeState(LEVEL_STATE::PLAY);

	wstring Level_Name = Cur_Level->GetName();
	if (Level_Name == LEVEL_LOBBY) {
		pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Zerohour [1].wav");
		pSound->Play(0, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}
	else if (Level_Name == LEVEL_0) {
		pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Metallurgy [1].wav");
		pSound->Play(0, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}
	else if (Level_Name == LEVEL_BOSS) {
		pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Defect [1].wav");
		pSound->Play(0, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}
}

void CGeneratorScript::Tick()
{
	if (SpawnvictoryScreen) {
		victoryScreen_Time += DT;
	}

	if (victoryScreen_Time > 3.f) {
		GeneratorChangeLevel();
	}

	CLevel* pPlayerLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pPlayerLayer = pPlayerLevel->GetLayer(5); //PlayerLayer = 5
	CGameObject* pPlayer = new CGameObject;
	const vector<CGameObject*>& vecPlayerObj = pPlayerLayer->GetParentObjects();
	for (int j = 0; j < vecPlayerObj.size(); j++) {
		if (vecPlayerObj[j]->GetName() == L"pBody") {
			pPlayer = vecPlayerObj[j];
		}
	}


	CGameObject* m_CurObject = this->GetOwner();

	m_ReneratorUI_state = m_CurObject->GetGenerator_ui_state();

	Part_Come1 = m_CurObject->GetHavePart1();
	Part_Come2 = m_CurObject->GetHavePart2();
	Part_Want1 = m_CurObject->GetWantPart1();
	Part_Want2 = m_CurObject->GetWantPart2();

	if (m_CurObject->GetName() == L"Generator_0" || m_CurObject->GetName() == L"Generator_1") {
		m_CurObject->FlipbookRender()->SetDir(1);
	}
	else if (m_CurObject->GetName() == L"Generator_2" || m_CurObject->GetName() == L"Generator_3") {
		m_CurObject->FlipbookRender()->SetDir(0);
	}

	//###################################
	//발전기 UI 보이는 부분
	//###################################

	//UI 처음 업데이트
	//if (!m_CurObject->GetSpawnPart()) {

	//pWant 소환

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pLayer = pLevel->GetLayer(4);
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	CGameObject* pWant = new CGameObject;
	Ptr<CMaterial> pMtrl;

	CGameObject* pWant2 = new CGameObject;
	Ptr<CMaterial> pMtrl2;

	for (int j = 0; j < vecObj.size(); j++) {
		//objNameStr.c_str()는 std::string의 내부 버퍼를 반환하는데, std::vector에 추가되는 것이 아니라, 같은 메모리 주소를 가리키는 포인터를 추가하는 것이야.

		//objNameStr은 for문에서 계속 덮어쓰이므로, ItemList에는 모두 같은 주소가 저장됨.
		string objNameStr = ToString(vecObj[j]->GetName());  // wstring을 string으로 변환

		UINT a = vecObj[j]->GetID();
		UINT b = m_CurObject->GetID();

		if (m_CurObject->GetName() == L"Generator_0") {
			if (objNameStr == "WantPart_0_0") {
				pWant = vecObj[j];
				pWant->SetWant1(true);
			}
			if (objNameStr == "WantPart_0_1") {
				pWant2 = vecObj[j];
				pWant2->SetWant1(true);
			}
		}
		if (m_CurObject->GetName() == L"Generator_1") {
			if (objNameStr == "WantPart_1_0") {
				pWant = vecObj[j];
				pWant->SetWant1(true);
			}
			if (objNameStr == "WantPart_1_1") {
				pWant2 = vecObj[j];
				pWant2->SetWant1(true);
			}
		}
		if (m_CurObject->GetName() == L"Generator_2") {
			if (objNameStr == "WantPart_2_0") {
				pWant = vecObj[j];
				pWant->SetWant1(true);
			}
			if (objNameStr == "WantPart_2_1") {
				pWant2 = vecObj[j];
				pWant2->SetWant1(true);
			}
		}
		if (m_CurObject->GetName() == L"Generator_3") {
			if (objNameStr == "WantPart_3_0") {
				pWant = vecObj[j];
				pWant->SetWant1(true);
			}
			if (objNameStr == "WantPart_3_1") {
				pWant2 = vecObj[j];
				pWant2->SetWant1(true);
			}
		}

	}

	if (!pWant->GetWant()) {
		pWant = new CGameObject;
		if (m_CurObject->GetName() == L"Generator_0") {
			pWant->SetName(L"WantPart_0_0");
		}
		else if (m_CurObject->GetName() == L"Generator_1") {
			pWant->SetName(L"WantPart_1_0");
		}
		else if (m_CurObject->GetName() == L"Generator_2") {
			pWant->SetName(L"WantPart_2_0");
		}
		else if (m_CurObject->GetName() == L"Generator_3") {
			pWant->SetName(L"WantPart_3_0");
		}
		pWant->AddComponent(new CTransform);
		pWant->AddComponent(new CMeshRender);

		Vec3 Vpos = m_CurObject->Transform()->GetRelativePos();
		if (m_CurObject->FlipbookRender()->GetDir() == 0) {
			Vpos.x += 150.f;
		}
		else {
			Vpos.x -= 150.f;
		}
		Vpos.y += 30.f;
		Vpos.z = -10;
		pWant->Transform()->SetRelativePos(Vpos);
		pWant->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
		pWant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		SpawnGameObject(pWant, 4);
	}
	else {
		Vec3 Vpos = m_CurObject->Transform()->GetRelativePos();
		if (m_CurObject->FlipbookRender()->GetDir() == 0) {
			Vpos.x += 150.f;
		}
		else {
			Vpos.x -= 150.f;
		}
		Vpos.y += 30.f;
		Vpos.z = -10;
		pWant->Transform()->SetRelativePos(Vpos);
	}

	if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_NONE) {
		pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));
		pMtrl = pWant->MeshRender()->GetMaterial();
		pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
	}
	else if (m_CurObject->GetGeneratorCheck1()) {
		pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SuccessUI"));
		pMtrl = pWant->MeshRender()->GetMaterial();
		pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\SuccessUI.png"));
	}
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_QUESTION) {
		pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"QuestionUI"));
		pMtrl = pWant->MeshRender()->GetMaterial();
		pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\QuestionUI.png"));
	}
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_PART) {
		if (Part_Want1 == PART::CARBON) {
			pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CarbonWantUI"));
			pMtrl = pWant->MeshRender()->GetMaterial();
			pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CarbonWantUI.png"));
		}
		else if (Part_Want1 == PART::COPPER) {
			pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CopperWantUI"));
			pMtrl = pWant->MeshRender()->GetMaterial();
			pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CopperWantUI.png"));
		}
		else if (Part_Want1 == PART::GEAR) {
			pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GearWantUI"));
			pMtrl = pWant->MeshRender()->GetMaterial();
			pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\GearWantUI.png"));
		}
		else if (Part_Want1 == PART::MACHINED_CARBON) {
			pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_CarbonWantUI"));
			pMtrl = pWant->MeshRender()->GetMaterial();
			pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Machined_CarbonWantUI.png"));
		}
		else if (Part_Want1 == PART::MACHINED_COPPER) {
			pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_CopperWantUI"));
			pMtrl = pWant->MeshRender()->GetMaterial();
			pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Machined_CopperWantUI.png"));
		}
	}

	pMtrl->SetScalarParam(INT_0, 0);

	if (!pWant2->GetWant()) {
		//pWant 소환2
		pWant2 = new CGameObject;
		if (m_CurObject->GetName() == L"Generator_0") {
			pWant2->SetName(L"WantPart_0_1");
		}
		else if (m_CurObject->GetName() == L"Generator_1") {
			pWant2->SetName(L"WantPart_1_1");
		}
		else if (m_CurObject->GetName() == L"Generator_2") {
			pWant2->SetName(L"WantPart_2_1");
		}
		else if (m_CurObject->GetName() == L"Generator_3") {
			pWant2->SetName(L"WantPart_3_1");
		}
		pWant2->AddComponent(new CTransform);
		pWant2->AddComponent(new CMeshRender);

		Vec3 Vpos2 = m_CurObject->Transform()->GetRelativePos();
		if (m_CurObject->FlipbookRender()->GetDir() == 0) {
			Vpos2.x += 150.f;
		}
		else {
			Vpos2.x -= 150.f;
		}
		Vpos2.y -= 50.f;
		Vpos2.z = -10;

		pWant2->Transform()->SetRelativePos(Vpos2);
		pWant2->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
		pWant2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

		SpawnGameObject(pWant2, 4);
		m_CurObject->SetSpawnPart(true);

	}
	else {
		Vec3 Vpos2 = m_CurObject->Transform()->GetRelativePos();
		if (m_CurObject->FlipbookRender()->GetDir() == 0) {
			Vpos2.x += 150.f;
		}
		else {
			Vpos2.x -= 150.f;
		}
		Vpos2.y -= 50.f;
		Vpos2.z = -10;
		pWant2->Transform()->SetRelativePos(Vpos2);
	}

	if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_NONE) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
	}
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_QUESTION) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"QuestionUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\QuestionUI.png"));

	}
	else if (m_CurObject->GetWantPart2() == PART::NONE_HAND) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
	}
	else if (m_CurObject->GetGeneratorCheck2()) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SuccessUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\SuccessUI.png"));
	}
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_PART) {
		if (Part_Want2 == PART::CARBON) {
			pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CarbonWantUI"));
			pMtrl2 = pWant2->MeshRender()->GetMaterial();
			pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CarbonWantUI.png"));
		}
		else if (Part_Want2 == PART::COPPER) {
			pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"CopperWantUI"));
			pMtrl2 = pWant2->MeshRender()->GetMaterial();
			pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\CopperWantUI.png"));
		}
		else if (Part_Want2 == PART::GEAR) {
			pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"GearWantUI"));
			pMtrl2 = pWant2->MeshRender()->GetMaterial();
			pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\GearWantUI.png"));
		}
		else if (Part_Want2 == PART::MACHINED_CARBON) {
			pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_CarbonWantUI"));
			pMtrl2 = pWant2->MeshRender()->GetMaterial();
			pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Machined_CarbonWantUI.png"));
		}
		else if (Part_Want2 == PART::MACHINED_COPPER) {
			pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_CopperWantUI"));
			pMtrl2 = pWant2->MeshRender()->GetMaterial();
			pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\Machined_CopperWantUI.png"));
		}
	}

	pMtrl2->SetScalarParam(INT_0, 0);



	//###################################
	//발전기 고치는 로직 부분
	//###################################

	//Broken Generator Start Flipbook이 끝나면 Broken Generator Ing Flipbook 재생 
	if (m_Generator_state == GENERATOR_STATE::GENERATOR_BREAK_START && m_CurObject->FlipbookRender()->IsFinish()) {
		m_Generator_state = GENERATOR_STATE::GENERATOR_BREAK_ING;
		m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_BREAK_ING, 10, true);

		//Part want1
		//Part want2 세팅
		random_device rd;
		random_device rd2;
		mt19937 gen(rd());
		mt19937 gen2(rd2());
		//uniform_int_distribution<> dis(PART::GEAR, PART::MACHINED_CARBON);
		//uniform_int_distribution<> dis2(PART::NONE_HAND, PART::MACHINED_CARBON);
		//GEAR,
		//COPPER,
		//CARBON,
		//MACHINED_COPPER,
		//MACHINED_CARBON,
		//PART_END

		int RandomPart1 = 0;
		int RandomPart2 = 0;

		if (pPlayerLevel->GetName() == LEVEL_LOBBY) {
			uniform_int_distribution<> dis(PART::GEAR, PART::MACHINED_CARBON);
			uniform_int_distribution<> dis2(PART::NONE_HAND, PART::MACHINED_CARBON);
			RandomPart1 = dis(gen);
			RandomPart2 = dis2(gen2);
		}
		else if (pPlayerLevel->GetName() == LEVEL_0) {
			uniform_int_distribution<> dis(PART::GEAR, PART::COPPER);
			uniform_int_distribution<> dis2(PART::NONE_HAND, PART::COPPER);
			RandomPart1 = dis(gen);
			RandomPart2 = dis2(gen2);
		}
		else if (pPlayerLevel->GetName() == LEVEL_BOSS) {
			uniform_int_distribution<> dis(PART::COPPER, PART::MACHINED_CARBON);
			uniform_int_distribution<> dis2(PART::COPPER, PART::MACHINED_CARBON);
			RandomPart1 = dis(gen);
			RandomPart2 = dis2(gen2);
		}



		m_CurObject->SetWantPart1((PART)RandomPart1);
		m_CurObject->SetWantPart2((PART)RandomPart2);

		if (pGenerator_Effect == nullptr && pGenerator_Effect_spawn == false) {
			pGenerator_Effect = new CGameObject;
			pGenerator_Effect->SetName(L"GeneratorSpark");
			pGenerator_Effect->AddComponent(new CTransform);
			pGenerator_Effect->AddComponent(new CFlipbookRender);
			Vec3 a = m_CurObject->Transform()->GetWorldPos();
			a.z = -30;
			a.y += 20.f;
			a.x -= 20.f;

			pGenerator_Effect->Transform()->SetRelativePos(a);
			pGenerator_Effect->Transform()->SetRelativeScale(300, 300.f, 1.f);
			pGenerator_Effect->Transform()->SetIndependentScale(true);

			pGenerator_Effect->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Generator_Explosion.flip"));
			pGenerator_Effect->FlipbookRender()->Play(0, 10, false);
			SpawnGameObject(pGenerator_Effect, 4);
			pGenerator_Effect_spawn = true;
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_break_01_L [1].wav");
			pSound->Play(1, 0.2f, false);
		}

	}
	//Broken Generator Ing Flipbook이 재생되는 상태에서 원하는 부품을 가져왔을때
	//Fix start 재생
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_BREAK_ING) {
		//m_CurObject->SetGeneratorUIUpdate(true);
		//원하는 부품을 가져왔을때

		//Generator_Effect Spark 가 삭제되어있을때
		pGenerator_Effect_CoolTime += DT;

		if (pGenerator_Effect != nullptr) {
			if (pGenerator_Effect->FlipbookRender()->IsFinish()) {
				pGenerator_Effect->Destroy();
				pGenerator_Effect = nullptr;
				pGenerator_Effect_CoolTime = 0.f;
			}
		}
		else if (pGenerator_Effect_CoolTime > 2.f && pGenerator_Effect == nullptr) {

			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dis(0, 2);

			pGenerator_Effect = new CGameObject;
			pGenerator_Effect->SetName(L"GeneratorSpark");
			pGenerator_Effect->AddComponent(new CTransform);
			pGenerator_Effect->AddComponent(new CFlipbookRender);
			Vec3 a = m_CurObject->Transform()->GetWorldPos();
			a.z = -30;
			a.y += 20.f;
			a.x -= 20.f;

			pGenerator_Effect->Transform()->SetRelativePos(a);
			pGenerator_Effect->Transform()->SetRelativeScale(150, 150.f, 1.f);
			pGenerator_Effect->Transform()->SetIndependentScale(true);
			int Ran = dis(gen);
			if (Ran == 0) {
				pGenerator_Effect->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Spart_Type_A.flip"));
			}
			else if (Ran == 1) {
				pGenerator_Effect->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Spart_Type_B.flip"));
			}
			else if (Ran == 2) {
				pGenerator_Effect->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Spart_Type_Eletric.flip"));
			}

			pGenerator_Effect->FlipbookRender()->Play(0, 10, false);
			SpawnGameObject(pGenerator_Effect, 4);
			pGenerator_Effect_spawn = true;
		}


		if (m_ReneratorUI_state == GENERATOR_UI_PART) {
			if (m_CurObject->GetHavePart1() == m_CurObject->GetWantPart1())
			{
				//m_CurObject->SetGeneratorCheck1(true);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_yellow_battery_01_L [1].wav");
				pSound->Play(1, 0.2f, false);
			}

			else if (m_CurObject->GetHavePart1() == m_CurObject->GetWantPart2())
			{
				//m_CurObject->SetGeneratorCheck2(true);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_yellow_battery_01_L [1].wav");
				pSound->Play(1, 0.2f, false);
			}

			else if (m_CurObject->GetHavePart2() == m_CurObject->GetWantPart2())
			{
				//m_CurObject->SetGeneratorCheck2(true);
				m_CurObject->SetHavePart2(PART::NONE_HAND);
				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_yellow_battery_01_L [1].wav");
				pSound->Play(1, 0.2f, false);
			}

			else if (m_CurObject->GetHavePart2() == m_CurObject->GetWantPart1())
			{
				//m_CurObject->SetGeneratorCheck1(true);
				m_CurObject->SetHavePart2(PART::NONE_HAND);
				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_yellow_battery_01_L [1].wav");
				pSound->Play(1, 0.2f, false);
			}


			if (m_CurObject->GetGeneratorCheck1() && m_CurObject->GetGeneratorCheck2() && m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_PART) {
				m_Generator_state = GENERATOR_STATE::GENERATOR_FIX_START;
				m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_FIX_START, 10, false);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
				//m_CurObject->SetUiSpawnPart(false);
				m_CurObject->SetGenerator_ui_state(GENERATOR_UI_STATE::GENERATOR_UI_SUCCESS);

				//발전기 고쳐졌을때 만약 현재 레벨에서 필요한 발전기 수리 횟수를 다 채웠으면
				//FixAnimation이 나오면서 다음 레벨로 이동해야함

				//1. Player에게 현재 레벨에서 고쳐진 발전기 횟수 가져오기
				pPlayer->SetRepairCountUp();
				int RepairCount = pPlayer->GetRepairCount();
				int LimitRepairCount = 0;
				//2. 만약 현재 레벨에 알맞는 발전기 수리 횟수를 가졌으면 FixAnimatio 재생
				if (pPlayerLevel->GetName() == LEVEL_LOBBY) {
					LimitRepairCount = 1;
				}
				else if (pPlayerLevel->GetName() == LEVEL_0) {
					LimitRepairCount = 3;
				}
				else if (pPlayerLevel->GetName() == LEVEL_BOSS) {
					LimitRepairCount = 8;
				}
				if (RepairCount >= LimitRepairCount) {
					if (FixAnima == nullptr) {
						FixAnima = new CGameObject;
						FixAnima->SetName(L"FixAmination");
						FixAnima->AddComponent(new CTransform);
						FixAnima->AddComponent(new CFlipbookRender);
						Vec3 AnimaPos = GetOwner()->Transform()->GetRelativePos();
						AnimaPos.z -= 3;
						FixAnima->Transform()->SetRelativePos(AnimaPos);
						FixAnima->Transform()->SetRelativeScale(200, 340.f, 1.f);
						FixAnima->Transform()->SetIndependentScale(true);
						FixAnima->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\FixAnima.flip"));
						FixAnima->FlipbookRender()->Play(0, 8, false);
						SpawnGameObject(FixAnima, 4);

						Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\ui_trans_repair_complete_01 [1].wav");
						pSound->Play(1, 0.2f, false);
					}
				}

			}
			m_CurObject->SetTag(false);
		}
	}
	//Fix start가 끝났을때 Fix Ing 재생
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_FIX_START && m_CurObject->FlipbookRender()->IsFinish()) {
		m_Generator_state = GENERATOR_STATE::GENERATOR_FIX_ING;
		m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_FIX_ING, 10, true);
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_repair_01_L [1].wav");
		pSound->Play(1, 0.2f, false);

	}
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_FIX_ING) {
		if (FixAnima != nullptr) {
			if (FixAnima->FlipbookRender()->IsFinish()) {
				FixAnima->Destroy();
				FixAnima = nullptr;
				
				FixVFX = new CGameObject;
				FixVFX->SetName(L"FixVFX");
				FixVFX->AddComponent(new CTransform);
				FixVFX->AddComponent(new CFlipbookRender);
				FixVFX->Transform()->SetRelativePos(0,0,-100);
				FixVFX->Transform()->SetRelativeScale(1920, 1850.f, 1.f);
				FixVFX->Transform()->SetIndependentScale(true);
				FixVFX->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\GenVFX.flip"));
				FixVFX->FlipbookRender()->Play(0, 8, false);
				SpawnGameObject(FixVFX, 4);
			}
		}

		if (FixVFX != nullptr) {
			if (FixVFX->FlipbookRender()->IsFinish()) {
				//ChangeLevel();
				CGameObject* BlackBackground = new CGameObject;
				BlackBackground->SetName(L"BlackBackground");
				BlackBackground->AddComponent(new CTransform);
				BlackBackground->AddComponent(new CMeshRender);
				BlackBackground->Transform()->SetRelativePos(0.f, 0.f, -80.f);
				BlackBackground->Transform()->SetRelativeScale(1980.f, 1500.f, 1.f);
				BlackBackground->Transform()->SetIndependentScale(true);
				BlackBackground->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
				BlackBackground->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlackBackground"));
				pMtrl = BlackBackground->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\BlackBackground.png"));
				pMtrl->SetScalarParam(INT_0, 0);
				SpawnGameObject(BlackBackground, 5);

				CGameObject* victoryScreen = new CGameObject;
				victoryScreen->SetName(L"victoryScreen");
				victoryScreen->AddComponent(new CTransform);
				victoryScreen->AddComponent(new CMeshRender);
				victoryScreen->Transform()->SetRelativePos(0.f, 0.f, -100.f);
				victoryScreen->Transform()->SetRelativeScale(800.f, 424.f, 1.f);
				victoryScreen->Transform()->SetIndependentScale(true);

				victoryScreen->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
				victoryScreen->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"victoryScreen"));

				pMtrl = victoryScreen->MeshRender()->GetMaterial();
				pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\victoryScreen.png"));
				pMtrl->SetScalarParam(INT_0, 0);

				//PlayerObject 소환
				SpawnGameObject(victoryScreen, 5);
				SpawnvictoryScreen = true;
				FixVFX->Destroy();
				FixVFX = nullptr;
			}
		}

		ScriptRunningTime = m_CurObject->GetRunningTime();
		ScriptRunningTime += DT;
		if (ScriptRunningTime >= 10.f) {
			ScriptRunningTime = 0.f;
			m_Generator_state = GENERATOR_STATE::GENERATOR_BREAK_START;
			m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_BREAK_START, 10, false);
			m_CurObject->SetGenerator_ui_state(GENERATOR_UI_STATE::GENERATOR_UI_QUESTION);

			m_CurObject->SetGeneratorCheck1(false);
			m_CurObject->SetGeneratorCheck2(false);

			m_CurObject->SetHavePart1(PART::NONE_HAND);
			m_CurObject->SetHavePart2(PART::NONE_HAND);

		}
		m_CurObject->SetRunningTime(ScriptRunningTime);
		m_CurObject->SetSpawnPart(true);
		pGenerator_Effect_spawn = false;
		pGenerator_Effect_CoolTime = 0.f;
		//m_CurObject->SetUiSpawnPart(true);

	}

}

void CGeneratorScript::SaveToFile(FILE* _File)
{
	fwrite(&ScriptRunningTime, sizeof(float), 1, _File);
	fwrite(&m_Generator_state, sizeof(GENERATOR_STATE), 1, _File);
	fwrite(&m_ReneratorUI_state, sizeof(GENERATOR_UI_STATE), 1, _File);
	fwrite(&Part_Come1, sizeof(PART), 1, _File);
	fwrite(&Part_Come2, sizeof(PART), 1, _File);
	fwrite(&Part_Want1, sizeof(PART), 1, _File);
	fwrite(&Part_Want2, sizeof(PART), 1, _File);
	fwrite(&dir, sizeof(int), 1, _File);

	//SaveAssetRef(m_Prefab, _File);
}


void CGeneratorScript::LoadFromFile(FILE* _File)
{
	fread(&ScriptRunningTime, sizeof(float), 1, _File);
	fread(&m_Generator_state, sizeof(GENERATOR_STATE), 1, _File);
	fread(&m_ReneratorUI_state, sizeof(GENERATOR_UI_STATE), 1, _File);
	fread(&Part_Come1, sizeof(PART), 1, _File);
	fread(&Part_Come2, sizeof(PART), 1, _File);
	fread(&Part_Want1, sizeof(PART), 1, _File);
	fread(&Part_Want2, sizeof(PART), 1, _File);
	fread(&dir, sizeof(int), 1, _File);
}

void CGeneratorScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m_CurObject = this->GetOwner();
	if (_Other->GetName() == L"pBody" && KEY_TAP(ENTER)) {

		m_ReneratorUI_state = m_CurObject->GetGenerator_ui_state();

		if (m_ReneratorUI_state == GENERATOR_UI_STATE::GENERATOR_UI_PART) {
			m_CurObject->SetHavePart1(_Other->GetHavePart1());
			m_CurObject->SetHavePart2(_Other->GetHavePart2());
		}
		if (_Other->GetHavePart1() == m_CurObject->GetWantPart1() && !m_CurObject->GetGeneratorCheck1())
		{
			_Other->SetHavePart1(PART::NONE_HAND);
			m_CurObject->SetGeneratorCheck1(true);
		}

		if (_Other->GetHavePart1() == m_CurObject->GetWantPart2() && !m_CurObject->GetGeneratorCheck2())
		{
			_Other->SetHavePart1(PART::NONE_HAND);
			m_CurObject->SetGeneratorCheck2(true);
		}

		if (_Other->GetHavePart2() == m_CurObject->GetWantPart2() && !m_CurObject->GetGeneratorCheck2())
		{
			_Other->SetHavePart2(PART::NONE_HAND);
			m_CurObject->SetGeneratorCheck2(true);
		}

		if (_Other->GetHavePart2() == m_CurObject->GetWantPart1() && !m_CurObject->GetGeneratorCheck1())
		{
			_Other->SetHavePart2(PART::NONE_HAND);
			m_CurObject->SetGeneratorCheck1(true);
		}
	}
}

void CGeneratorScript::GeneratorChangeLevel()
{
	CLevel* Cur_Level = CLevelMgr::GetInst()->GetCurrentLevel();
	wstring Flie_Path = CPathMgr::GetContentPath();
	CLevel* pNextLevel = nullptr;

	pSound->Stop();
	wstring Level_Name = Cur_Level->GetName();
	if (Level_Name == LEVEL_LOBBY) {
		pNextLevel = LoadLevel(Flie_Path + L"Level\\Level_0_Save_24");
		pNextLevel->SetName(LEVEL_0);
		//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Metallurgy [1].wav");
		//pSound->Play(1, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}
	else if (Level_Name == LEVEL_0) {
		pNextLevel = LoadLevel(Flie_Path + L"Level\\Level_Boss_40");
		pNextLevel->SetName(LEVEL_BOSS);
		//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Defect [1].wav");
		//pSound->Play(1, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}
	else if (Level_Name == LEVEL_BOSS) {
		pNextLevel = LoadLevel(Flie_Path + L"Level\\Level_2_Save_0");
		pNextLevel->SetName(LEVEL_0);
		//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\Zerohour [1].wav");
		//pSound->Play(1, 0.2f, false);
		//pNextLevel->ChangeState(LEVEL_STATE::PLAY);
	}

	tTask ChangeStatetask = {};
	ChangeStatetask.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	ChangeStatetask.Param0 = (DWORD_PTR)LEVEL_STATE::PLAY;


	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)pNextLevel;
	

	CTaskMgr::GetInst()->AddTask(task);
	CTaskMgr::GetInst()->AddTask(ChangeStatetask);

	wstring strBinPath = CPathMgr::GetBinPath();
	SetCurrentDirectory(strBinPath.c_str());

}

CLevel* CGeneratorScript::LoadLevel(wstring _FilePath)
{
	CLevel* pNewLevel = new CLevel;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 레벨 이름 
	wstring LevelName;
	LoadWString(LevelName, pFile);

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer 이름
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// 오브젝트 개수
		size_t ObjectCount = 0;
		fread(&ObjectCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < ObjectCount; ++i)
		{
			CGameObject* pObject = LoadGameObject(pFile);
			pLayer->AddGameObject(pObject, false);
		}
	}

	fclose(pFile);

	return pNewLevel;
}

CGameObject* CGeneratorScript::LoadGameObject(FILE* _File)
{
	// 오브젝트 생성
	CGameObject* pNewObject = new CGameObject;

	// 오브젝트 이름
	wstring Name;
	LoadWString(Name, _File);
	pNewObject->SetName(Name);

	// 컴포넌트 정보
	while (true)
	{
		COMPONENT_TYPE ComType = COMPONENT_TYPE::COMPONENT_END;
		fread(&ComType, sizeof(COMPONENT_TYPE), 1, _File);

		if (COMPONENT_END == ComType)
			break;

		CComponent* pComponent = nullptr;

		switch (ComType)
		{
		case TRANSFORM:
			pComponent = new CTransform;
			break;
		case COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COLLIDER3D:
			//pComponent = new CCollider3D;
			break;
		case LIGHT2D:
			pComponent = new CLight2D;
			break;
		case LIGHT3D:
			//pComponent = new CLight3D;
			break;
		case CAMERA:
			pComponent = new CCamera;
			break;
		case BOUNDINGBOX:
			//pComponent = new CBoundingBox;
			break;
		case MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case SPRITERENDER:
			pComponent = new CSpriteRender;
			break;
		case FLIPBOOKRENDER:
			pComponent = new CFlipbookRender;
			break;
		case TILERENDER:
			pComponent = new CTileRender;
			break;
		case PARTICLERENDER:
			pComponent = new CParticleRender;
			break;
		case SKYBOX:
			//pComponent = new CSkyBox;
			break;
		case DECAL:
			//pComponent = new CDecal;
			break;
		case LANDSCAPE:
			//pComponent = new CLandScape;
			break;
		}

		// 컴포넌트 이름 불러오기
		wstring Name;
		LoadWString(Name, _File);
		pComponent->SetName(Name);

		// 생성한 컴포넌트를 오브젝트에 추가 및 저장데이터 로딩
		pNewObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// ====================
	// 스크립트 정보 불러오기
	// ====================
	// 스크립트 개수
	int Size = 0;
	fread(&Size, sizeof(int), 1, _File);

	for (size_t i = 0; i < Size; ++i)
	{
		// 스크립트의 종류(이름)
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// 스크립트 이름으로 스크립트를 생성시켜서 오브젝트에 넣어줌
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pNewObject->AddComponent(pScript);

		// 스크립트가 저장한 데이터를 다시 복구함
		pScript->LoadFromFile(_File);
	}

	// ====================
	// 자식 오브젝트 불러오기
	// ====================
	int ChildCount = 0;
	fread(&ChildCount, sizeof(int), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* pChildObj = LoadGameObject(_File);
		pNewObject->AddChild(pChildObj);
	}

	return pNewObject;
}
