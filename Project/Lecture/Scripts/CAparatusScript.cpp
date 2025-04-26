#include "pch.h"
#include "CAparatusScript.h"

#include <Engine/components.h>

#include <Engine/CFlipbookRender.h>
#include <Engine/CGameObject.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CKeyMgr.h>

CAparatusScript::CAparatusScript()
	:CScript(APARATUSSCRIPT),
	m_Apparatus_state(APPARATUS_STATE::APPARATUS_END),
	RunningTime(0.f),
	SpawnPart(false),
	pPart(nullptr)
{
}

CAparatusScript::~CAparatusScript()
{
}

void CAparatusScript::Init()
{
}

void CAparatusScript::Begin()
{
	int a = 0;
}

void CAparatusScript::Tick()
{

	int a = 0;
	CGameObject* m_CurObject = this->GetOwner();
	wstring APname = m_CurObject->GetName();
	m_Apparatus_state = this->GetOwner()->GetAppState();


	if (APname == L"Gear_Aparatus" || APname == L"Copper_Aparatus") {
		//현재 생성기의 상태를 가져옴

		if (m_Apparatus_state == APPARATUS_END && m_CurObject->GetTag()) {
			//만져지면 플레이어가 가져갔으니 Destory
			pPart->Destroy();
			m_CurObject->SetAppState(APPARATUS_READY);
			m_CurObject->SetTag(false);
			m_CurObject->SetSpawnPart(false);
		}
		else if (m_Apparatus_state == APPARATUS_READY && m_CurObject->GetTag()) {
			if (m_CurObject->FlipbookRender()->IsFinish()) {
				m_CurObject->SetAppState(APPARATUS_START);
				m_CurObject->FlipbookRender()->Play(APPARATUS_START, 10, false);
				m_CurObject->SetTag(false);
			}
		}
		else if (m_Apparatus_state == APPARATUS_START) {
			if (m_CurObject->FlipbookRender()->IsFinish()) {
				m_CurObject->SetAppState(APPARATUS_ING);
				m_CurObject->FlipbookRender()->Play(APPARATUS_ING, 10, true);
			}
		}
		else if (m_Apparatus_state == APPARATUS_ING) {
			RunningTime += DT;
			if (RunningTime > 7.f) {
				RunningTime = 0.f;
				m_CurObject->SetAppState(APPARATUS_END);
				m_CurObject->FlipbookRender()->Play(APPARATUS_END, 10, false);
				SpawnPart = false;
			}
		}
		else if (m_Apparatus_state == APPARATUS_END && m_CurObject->FlipbookRender()->IsFinish()) {
			//End 상태에서 플레이어에게 제공할 Part
			bool CurPartcheck = m_CurObject->GetSpawnPart();
			if (CurPartcheck == false) {
				//SpawnPart = true;
				pPart = new CGameObject;
				pPart->SetName(L"Part");
				pPart->AddComponent(new CTransform);
				pPart->AddComponent(new CMeshRender);
				Vec3 PartPos;
				//##################
				//위치 정하기
				//##################
				if (m_CurObject->GetName() == L"Gear_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 38;
				}
				else if (m_CurObject->GetName() == L"Copper_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}
				else if (m_CurObject->GetName() == L"Carbon_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}
				else if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}
				else if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}


				pPart->Transform()->SetRelativePos(PartPos);
				pPart->Transform()->SetRelativeScale(50.f, 50.f, 1.f);

				pPart->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));

				//##################
				//머터리얼 정하기
				//##################
				if (m_CurObject->GetName() == L"Gear_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Gear"));
				}
				else if (m_CurObject->GetName() == L"Copper_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Copper"));
				}
				else if (m_CurObject->GetName() == L"Carbon_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Carbon"));
				}
				else if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_Copper"));
				}
				else if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_Carbon"));
				}

				Ptr<CMaterial> pMtrl = pPart->MeshRender()->GetMaterial();

				//##################
				//텍스쳐 정하기
				//##################

				if (m_CurObject->GetName() == L"Gear_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0006.png"));
				}
				else if (m_CurObject->GetName() == L"Copper_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0007.png"));
				}
				else if (m_CurObject->GetName() == L"Carbon_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0009.png"));
				}
				else if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0008.png"));
				}
				else if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0010.png"));
				}

				pMtrl->SetScalarParam(INT_0, 0);
				SpawnGameObject(pPart, 4);
				m_CurObject->SetSpawnPart(true);
			}
		}
	}
	else if (APname == L"Machined_Copper_Aparatus" || APname == L"Machined_Carbon_Aparatus") {
		if (m_Apparatus_state == APPARATUS_END && m_CurObject->GetTag()) {
			//만져지면 플레이어가 가져갔으니 Destory
			pPart->Destroy();
			m_CurObject->SetAppState(APPARATUS_READY);
			m_CurObject->SetTag(false);
			m_CurObject->SetSpawnPart(false);
		}
		//가공 구리하고 가공 카본은 플레이어가 구리하고 카본을 가져와야 제작이 시작됨
		else if (m_Apparatus_state == APPARATUS_READY && m_CurObject->GetTag()) {
			if (APname == L"Machined_Copper_Aparatus") {
				if (m_CurObject->GetHavePart1() == PART::COPPER) {
					if (m_CurObject->FlipbookRender()->IsFinish()) {
						m_CurObject->SetAppState(APPARATUS_START);
						m_CurObject->FlipbookRender()->Play(APPARATUS_START, 10, false);
						m_CurObject->SetTag(false);
					}
				}
			}
			else if (APname == L"Machined_Carbon_Aparatus") {
				if (m_CurObject->GetHavePart1() == PART::CARBON) {
					if (m_CurObject->FlipbookRender()->IsFinish()) {
						m_CurObject->SetAppState(APPARATUS_START);
						m_CurObject->FlipbookRender()->Play(APPARATUS_START, 10, false);
						m_CurObject->SetTag(false);
					}
				}
			}
		}
		else if (m_Apparatus_state == APPARATUS_START) {
			if (m_CurObject->FlipbookRender()->IsFinish()) {
				m_CurObject->SetAppState(APPARATUS_ING);
				m_CurObject->FlipbookRender()->Play(APPARATUS_ING, 10, true);
			}
		}
		else if (m_Apparatus_state == APPARATUS_ING) {
			RunningTime += DT;
			if (RunningTime > 7.f) {
				RunningTime = 0.f;
				m_CurObject->SetAppState(APPARATUS_END);
				m_CurObject->FlipbookRender()->Play(APPARATUS_END, 10, false);
				SpawnPart = false;
			}
		}
		else if (m_Apparatus_state == APPARATUS_END && m_CurObject->FlipbookRender()->IsFinish()) {
			//End 상태에서 플레이어에게 제공할 Part
			bool CurPartcheck = m_CurObject->GetSpawnPart();
			if (CurPartcheck == false) {
				//SpawnPart = true;
				pPart = new CGameObject;
				pPart->SetName(L"Part");
				pPart->AddComponent(new CTransform);
				pPart->AddComponent(new CMeshRender);
				Vec3 PartPos;
				//##################
				//위치 정하기
				//##################
				if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}
				else if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					PartPos = m_CurObject->Transform()->GetRelativePos();
					PartPos.z = -1;
					PartPos.y -= 35;
				}


				pPart->Transform()->SetRelativePos(PartPos);
				pPart->Transform()->SetRelativeScale(50.f, 50.f, 1.f);

				pPart->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));

				//##################
				//머터리얼 정하기
				//##################
				if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_Copper"));
				}
				else if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					pPart->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Machined_Carbon"));
				}

				Ptr<CMaterial> pMtrl = pPart->MeshRender()->GetMaterial();

				//##################
				//텍스쳐 정하기
				//##################

				if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0008.png"));
				}
				else if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus") {
					pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\parts0010.png"));
				}

				pMtrl->SetScalarParam(INT_0, 0);
				SpawnGameObject(pPart, 4);
				m_CurObject->SetSpawnPart(true);
				m_CurObject->SetHavePart1(PART::NONE_HAND);
			}
		}
	}
	//Carbon_Aparatus
	else {
		if (m_Apparatus_state == APPARATUS_END && m_CurObject->GetTag()) {
			//만져지면 플레이어가 가져갔으니 Destory
			//Carbon은 그냥 바로 건네줌
			m_CurObject->SetAppState(APPARATUS_ING);
			m_CurObject->SetTag(false);
			m_CurObject->SetSpawnPart(false);
			m_CurObject->FlipbookRender()->Play(1, 10, false); //1이 Carbon Ing임
		}
		else if (m_Apparatus_state == APPARATUS_ING && m_CurObject->FlipbookRender()->IsFinish()) {
			m_CurObject->SetAppState(APPARATUS_END);
			m_CurObject->FlipbookRender()->Play(0, 10, false);
		}
	}
}

void CAparatusScript::PlayStartFlipbook()
{
}

void CAparatusScript::PlayIngFlipbook()
{
}

void CAparatusScript::PlayEndFlipbook()
{
}

void CAparatusScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	int a = 0;
}

void CAparatusScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m_CurObject = this->GetOwner();
	if (_Other->GetName() == L"pHead" && KEY_TAP(ENTER)) {

		if (m_CurObject->GetName() == L"Machined_Copper_Aparatus") {
			if (m_Apparatus_state == APPARATUS_READY) {
				if (_Other->GetHavePart1() == PART::COPPER)
				{
					m_CurObject->SetHavePart1(PART::COPPER);
					_Other->SetHavePart1(PART::NONE_HAND);
				}
				else if (_Other->GetHavePart2() == PART::COPPER)
				{
					m_CurObject->SetHavePart1(PART::COPPER);
					_Other->SetHavePart2(PART::NONE_HAND);
				}
			}

		}

		if (m_CurObject->GetName() == L"Machined_Carbon_Aparatus")
		{
			if (m_Apparatus_state == APPARATUS_READY) {
				if (_Other->GetHavePart1() == PART::CARBON)
				{
					m_CurObject->SetHavePart1(PART::CARBON);
					_Other->SetHavePart1(PART::NONE_HAND);
				}
				else if (_Other->GetHavePart2() == PART::CARBON)
				{
					m_CurObject->SetHavePart1(PART::CARBON);
					_Other->SetHavePart2(PART::NONE_HAND);
				}
			}

		}

	}
}

void CAparatusScript::SaveToFile(FILE* _File)
{
	fwrite(&SpawnPart, sizeof(bool), 1, _File);
	fwrite(&RunningTime, sizeof(float), 1, _File);
}

void CAparatusScript::LoadFromFile(FILE* _File)
{
	fread(&SpawnPart, sizeof(bool), 1, _File);
	fread(&RunningTime, sizeof(float), 1, _File);
}


