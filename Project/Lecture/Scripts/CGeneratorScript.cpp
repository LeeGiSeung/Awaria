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
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbookRender.h>

//랜덤 함수 사용
#include <random>

CGeneratorScript::CGeneratorScript()
	:CScript(GENERATORSCRIPT)
	, Part_Come1(PART::NONE_HAND)
	, Part_Come2(PART::NONE_HAND)
	, Part_Want1(PART::NONE_HAND)
	, Part_Want2(PART::NONE_HAND)
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
}

void CGeneratorScript::Tick()
{
	CGameObject* m_CurObject = this->GetOwner();

	m_ReneratorUI_state = m_CurObject->GetGenerator_ui_state();

	Part_Come1 = m_CurObject->GetHavePart1();
	Part_Come2 = m_CurObject->GetHavePart2();
	Part_Want1 = m_CurObject->GetWantPart1();
	Part_Want2 = m_CurObject->GetWantPart2();
	int a = 0;

	//###################################
	//발전기 UI 보이는 부분
	//###################################

	//UI 처음 업데이트
	//if (!m_CurObject->GetSpawnPart()) {
	CGameObject* pWant = new CGameObject;
	pWant->SetName(L"WantPart");
	pWant->AddComponent(new CTransform);
	pWant->AddComponent(new CMeshRender);

	Vec3 Vpos = m_CurObject->Transform()->GetRelativePos();
	Vpos.x += 150.f;
	Vpos.y += 30.f;
	Vpos.z = -2;
	Ptr<CMaterial> pMtrl;
	pWant->Transform()->SetRelativePos(Vpos);
	pWant->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	pWant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

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
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_QUESTION) {
		pWant->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"QuestionUI"));
		pMtrl = pWant->MeshRender()->GetMaterial();
		pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\QuestionUI.png"));
	}

	pMtrl->SetScalarParam(INT_0, 0);

	SpawnGameObject(pWant, 4);

	CGameObject* pWant2 = new CGameObject;
	pWant2->SetName(L"WantPart");
	pWant2->AddComponent(new CTransform);
	pWant2->AddComponent(new CMeshRender);

	Vec3 Vpos2 = m_CurObject->Transform()->GetRelativePos();
	Vpos2.x += 150.f;
	Vpos2.y -= 50.f;
	Vpos2.z = -2;

	pWant2->Transform()->SetRelativePos(Vpos2);
	pWant2->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	pWant2->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl2;

	if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_NONE) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NoneUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\NoneUI.png"));
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
	else if (m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_QUESTION) {
		pWant2->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"QuestionUI"));
		pMtrl2 = pWant2->MeshRender()->GetMaterial();
		pMtrl2->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\QuestionUI.png"));
	}

	pMtrl2->SetScalarParam(INT_0, 0);

	SpawnGameObject(pWant2, 4);
	m_CurObject->SetSpawnPart(true);
	pWant->Destroy();
	pWant2->Destroy();
//}

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
		uniform_int_distribution<> dis(PART::GEAR, PART::MACHINED_CARBON);
		uniform_int_distribution<> dis2(PART::NONE_HAND, PART::MACHINED_CARBON);

int RandomPart1 = dis(gen);
int RandomPart2 = dis2(gen2);

m_CurObject->SetWantPart1((PART)RandomPart1);
m_CurObject->SetWantPart2((PART)RandomPart2);

	}
	//Broken Generator Ing Flipbook이 재생되는 상태에서 원하는 부품을 가져왔을때
	//Fix start 재생
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_BREAK_ING) {
		//m_CurObject->SetGeneratorUIUpdate(true);
		//원하는 부품을 가져왔을때
		if (m_ReneratorUI_state == GENERATOR_UI_PART) {
			if (m_CurObject->GetHavePart1() == m_CurObject->GetWantPart1())
			{
				//m_CurObject->SetGeneratorCheck1(true);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
			}

			else if (m_CurObject->GetHavePart1() == m_CurObject->GetWantPart2())
			{
				//m_CurObject->SetGeneratorCheck2(true);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
			}

			else if (m_CurObject->GetHavePart2() == m_CurObject->GetWantPart2())
			{
				//m_CurObject->SetGeneratorCheck2(true);
				m_CurObject->SetHavePart2(PART::NONE_HAND);
			}

			else if (m_CurObject->GetHavePart2() == m_CurObject->GetWantPart1())
			{
				//m_CurObject->SetGeneratorCheck1(true);
				m_CurObject->SetHavePart2(PART::NONE_HAND);
			}


			if (m_CurObject->GetGeneratorCheck1() && m_CurObject->GetGeneratorCheck2() && m_CurObject->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_PART) {
				m_Generator_state = GENERATOR_STATE::GENERATOR_FIX_START;
				m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_FIX_START, 10, false);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
				m_CurObject->SetUiSpawnPart(false);
				m_CurObject->SetGenerator_ui_state(GENERATOR_UI_STATE::GENERATOR_UI_SUCCESS);
			}
			m_CurObject->SetTag(false);
		}
	}
	//Fix start가 끝났을때 Fix Ing 재생
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_FIX_START && m_CurObject->FlipbookRender()->IsFinish()) {
		m_Generator_state = GENERATOR_STATE::GENERATOR_FIX_ING;
		m_CurObject->FlipbookRender()->Play(GENERATOR_STATE::GENERATOR_FIX_ING, 10, true);
	}
	else if (m_Generator_state == GENERATOR_STATE::GENERATOR_FIX_ING) {
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
		m_CurObject->SetUiSpawnPart(true);

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
}

void CGeneratorScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m_CurObject = this->GetOwner();
	if (_Other->GetName() == L"pHead" && KEY_TAP(ENTER)) {

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
