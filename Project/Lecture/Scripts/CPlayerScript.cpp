#include "pch.h"
#include "CPlayerScript.h"

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
#include "CDroneScript.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbookRender.h>

#define Move 10
#define Three 3

//enum PLAYER_STATE {
//	PLAYER_IDLE,
//	PLAYER_MOVE,
//	PLAYER_DASH
//};

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_bBlockLeft(false)
	, m_bBlockRight(false)
	, m_bBlockDown(false)
	, m_bBlockUp(false)
	, m_PlayerUDDir(MOVE_DIR::MOVE_UP)
	, m_PlayerLRDir(MOVE_DIR::MOVE_LEFT)
	, m_Speed(1.f)
	, m_PaperBurn(0.f)
	, CUR_PLAYER_STATE(PLAYER_STATE::PLAYER_IDLE)
	, m_PlayerObject(nullptr)
	, bBlockCount(0)
	, PlayerHP_Drone(false)
	, PlayerHP(1)
	, Drone_CoolTime(6.f)
	, Invincibility(false)
	, Invincibility_Time(0.f)
	, Dash_Invincibility(false)
	, Dash_Invincibility_Time(0.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Init()
{

}

void CPlayerScript::Begin()
{

}
//CComponent* PlayerFlipbookRender = m_PlayerObject->GetComponent(COMPONENT_TYPE::FLIPBOOKRENDER);
//CFlipbookRender* FlipbookRender = dynamic_cast<CFlipbookRender*>(PlayerFlipbookRender);
//FlipbookRender->Play(CUR_PLAYER_STATE, 10, true);
void CPlayerScript::Tick()
{

	if (CUR_PLAYER_STATE == PLAYER_STATE::PLAYER_DASH) {
		Invincibility = true;
		Dash_Invincibility = true;
	}

	if (Dash_Invincibility) {
		Dash_Invincibility_Time += DT;
		Invincibility = true;
		if (Dash_Invincibility_Time > 1.f) {
			Dash_Invincibility_Time = 0.f;
			Invincibility = false;
			Dash_Invincibility = false;
		}


	}

	if (CUR_PLAYER_STATE == PLAYER_STATE::PLAYER_DEATH) {
		if (FlipbookRender()->IsFinish()) {
			//CLevel* pNextLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			//tTask task = {};
			//task.Type = TASK_TYPE::CHANGE_LEVEL;
			//task.Param0 = (DWORD_PTR)pNextLevel;

			//CTaskMgr::GetInst()->AddTask(task);
		}
	}
	else {
		if (PlayerHP_Drone == false) {
			Drone_CoolTime += DT;
		}

		if (Invincibility == true) {
			Invincibility_Time += DT;
		}

		if (Invincibility_Time > 3.f) {
			Invincibility = false;
			Invincibility_Time = 0.f;
		}

		//Player�� Drone�� ������ ���� ���� ��
		CLevel* pLevelDrone = CLevelMgr::GetInst()->GetCurrentLevel();

		CLayer* pLayerDrone = pLevelDrone->GetLayer(5);
		const vector<CGameObject*>& DroneList = pLayerDrone->GetParentObjects();

		for (int j = 0; j < DroneList.size(); j++) {

			if (DroneList[j]->GetName() == L"pDrone") {
				if (PlayerHP_Drone == false) {
					DroneList[j]->Destroy();
					Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_drone_destroy_01 [1].wav");
					pSound->Play(1, 0.2f, false);
					//DroneList[j]->SetPlayerHeat(true);
					break;
				}
			}
		}

		if (PlayerHP_Drone == false && Drone_CoolTime > 5.f && Invincibility == false) {

			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_drone_respawn_01 [1].wav");
			pSound->Play(1, 0.2f, false);

			CGameObject* pDrone = new CGameObject;
			pDrone->SetName(L"pDrone");
			pDrone->AddComponent(new CTransform);
			pDrone->AddComponent(new CFlipbookRender);
			pDrone->AddComponent(new CDroneScript);
			pDrone->Transform()->SetRelativePos(0.f, 0.f, -10.f);
			pDrone->Transform()->SetRelativeScale(56, 56.f, 1.f);
			pDrone->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Drone.flip"));
			pDrone->FlipbookRender()->AddFlipbook(1, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Drone_Death.flip"));
			pDrone->FlipbookRender()->Play(0, 10, true);
			SpawnGameObject(pDrone, 5);
			Drone_CoolTime = 0.f;
			PlayerHP_Drone = true;
		}

		if (bBlockCount == 0) {
			m_bBlockLeft = false;
			m_bBlockRight = false;
			m_bBlockUp = false;
			m_bBlockDown = false;
		}

		m_PlayerObject = this->GetOwner();
		CTransform* pTrans = GetOwner()->Transform();
		Vec3 vPos = pTrans->GetRelativePos();
		wstring ObjectName = m_PlayerObject->GetName();

		if (ObjectName == L"pBody") {

			Collider2D()->SetScale(Vec2(50.f, 50.f));

			if (KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::S)) {
				if (KEY_PRESSED(KEY::W) && !m_bBlockUp) {
					Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_footstep_01 [1].wav");
					pSound->Play(1, 0.1f, false);
					vPos.y += m_Speed * DT * 500;
					m_PlayerUDDir = MOVE_UP;
					if (KEY_PRESSED(KEY::A) && !m_bBlockLeft) {
						vPos.x -= m_Speed * DT * 500;
						m_PlayerLRDir = MOVE_LEFT;
					}
					if (KEY_PRESSED(KEY::D) && !m_bBlockRight) {
						vPos.x += m_Speed * DT * 500;
						m_PlayerLRDir = MOVE_RIGHT;
					}
				}
				if (KEY_PRESSED(KEY::S) && !m_bBlockDown) {
					vPos.y -= m_Speed * DT * 500;
					m_PlayerUDDir = MOVE_DOWN;
					Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_footstep_01 [1].wav");
					pSound->Play(1, 0.1f, false);
					if (KEY_PRESSED(KEY::A) && !m_bBlockLeft) {
						vPos.x -= m_Speed * DT * 500;
						m_PlayerLRDir = MOVE_LEFT;
					}
					if (KEY_PRESSED(KEY::D) && !m_bBlockRight) {
						vPos.x += m_Speed * DT * 500;
						m_PlayerLRDir = MOVE_RIGHT;
					}
				}
			}
			else {
				if (KEY_PRESSED(KEY::A) && !m_bBlockLeft) {
					vPos.x -= m_Speed * DT * 500;
					m_PlayerLRDir = MOVE_LEFT;
					Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_footstep_01 [1].wav");
					pSound->Play(1, 0.1f, false);
				}
				if (KEY_PRESSED(KEY::D) && !m_bBlockRight) {
					vPos.x += m_Speed * DT * 500;
					m_PlayerLRDir = MOVE_RIGHT;
					Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_footstep_01 [1].wav");
					pSound->Play(1, 0.1f, false);
				}
			}

			if (KEY_TAP(KEY::SPACE))
			{
				if (KEY_PRESSED(KEY::W) && !m_bBlockUp) {
					vPos.y += m_Speed * DT * 100000;
					m_PlayerUDDir = MOVE_UP;
				}
				if (KEY_PRESSED(KEY::A) && !m_bBlockLeft) {
					vPos.x -= m_Speed * DT * 100000;
					m_PlayerLRDir = MOVE_LEFT;
				}
				if (KEY_PRESSED(KEY::D) && !m_bBlockRight) {
					vPos.x += m_Speed * DT * 100000;
					m_PlayerLRDir = MOVE_RIGHT;
				}
				if (KEY_PRESSED(KEY::S) && !m_bBlockDown) {
					vPos.y -= m_Speed * DT * 100000;
					m_PlayerUDDir = MOVE_DOWN;
				}
			}
			if (KEY_TAP(KEY::Z)) {
				Vec3 CurPos = m_PlayerObject->Transform()->GetRelativePos();
				m_PlayerObject->Transform()->SetRelativePos(CurPos.x, CurPos.y + 10.f, CurPos.z);
			}

			if (CUR_PLAYER_STATE == PLAYER_STATE::PLAYER_DASH) {
				if (m_PlayerObject->FlipbookRender()->IsFinish()) {
					//������ �����ð��� ���� �����
					Dash_Invincibility = true;
					m_PlayerMoveCheckBool = false;
				}
			}

			m_PlayerObject = this->GetOwner();

			/*
			Player�̵�
			*/
			if (KEY_PRESSED(KEY::A)) {
				if (CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_MOVE && !m_PlayerMoveCheckBool) {
					CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_MOVE;
					m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 12, true);
				}
				m_PlayerMoveCheckBool = true;
			}
			if (KEY_PRESSED(KEY::D)) {
				if (CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_MOVE && !m_PlayerMoveCheckBool) {
					CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_MOVE;
					m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 12, true);
				}
				m_PlayerMoveCheckBool = true;
			}
			if (KEY_PRESSED(KEY::W)) {
				if (CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_MOVE && !m_PlayerMoveCheckBool) {
					CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_MOVE;
					m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 12, true);
				}
				m_PlayerMoveCheckBool = true;
			}
			if (KEY_PRESSED(KEY::S)) {
				if (CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_MOVE && !m_PlayerMoveCheckBool) {
					CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_MOVE;
					m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 12, true);
				}
				m_PlayerMoveCheckBool = true;
			}
			/*
			PlayerIdle ���·� ��ȯ
			*/
			if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S) || KEY_RELEASED(KEY::A) || KEY_RELEASED(KEY::D)) {
				m_PlayerMoveCheckBool = false;
			}
			if (!m_PlayerMoveCheckBool) {
				if (CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_IDLE) {
					CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_IDLE;
					m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 10, true);
				}
			}

			if (KEY_RELEASED(KEY::W) || KEY_RELEASED(KEY::S) || KEY_RELEASED(KEY::A) || KEY_RELEASED(KEY::D)) {
				m_PlayerMoveCheckBool = false;
			}

			if (KEY_TAP(KEY::SPACE))
			{
				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_dash_01 [1].wav");
				pSound->Play(1, 0.2f, false);
				if (KEY_PRESSED(KEY::W)) {
					vPos.y += m_Speed * DT * 500;
				}
				if (KEY_PRESSED(KEY::A)) {
					vPos.x -= m_Speed * DT * 500;
				}
				if (KEY_PRESSED(KEY::D)) {
					vPos.x += m_Speed * DT * 500;
				}
				if (KEY_PRESSED(KEY::S)) {
					vPos.y -= m_Speed * DT * 500;
				}
				CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_DASH;
				m_PlayerMoveCheckBool = true;
				m_PlayerObject->FlipbookRender()->Play(CUR_PLAYER_STATE, 10, true);
			}

			if (KEY_PRESSED(KEY::A)) {
				m_PlayerObject->FlipbookRender()->SetDir(0);
			}
			if (KEY_PRESSED(KEY::D)) {
				m_PlayerObject->FlipbookRender()->SetDir(1);
			}

		}
		if (ObjectName == L"pHead") {
			if (KEY_PRESSED(KEY::A)) {
				Ptr<CMaterial> pHeadMtrl = m_PlayerObject->MeshRender()->GetMaterial();
				pHeadMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaHead0002.png"));
			}
			if (KEY_PRESSED(KEY::D)) {
					Ptr<CMaterial> pHeadMtrl = m_PlayerObject->MeshRender()->GetMaterial();
					pHeadMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaHead0002Right.png"));
			}

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			CLayer* pLayer = pLevel->GetLayer(5);
			const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

			CGameObject* pBody = nullptr;

			for (int j = 0; j < vecObj.size(); j++) {
				//objNameStr.c_str()�� std::string�� ���� ���۸� ��ȯ�ϴµ�, std::vector�� �߰��Ǵ� ���� �ƴ϶�, ���� �޸� �ּҸ� ����Ű�� �����͸� �߰��ϴ� ���̾�.

				//objNameStr�� for������ ��� ����̹Ƿ�, ItemList���� ��� ���� �ּҰ� �����.
				string objNameStr = ToString(vecObj[j]->GetName());  // wstring�� string���� ��ȯ

				if (vecObj[j]->GetName() == L"pBody") {
					pBody = vecObj[j];
					break;
				}
			}

			//vPos = pBody->Transform()->GetRelativePos();

			//vPos.y += 40;
			//vPos.z -= 13;

			if (pBody != nullptr) {
				int CurFlipbook = pBody->FlipbookRender()->GetFlipbookIdx();
				int CurSprite = pBody->FlipbookRender()->GetSpriteIdx();

				// ���� �ν�: Flipbook/Sprite�� �ٲ�鸸 png �ε��� ����
				if (CurFlipbook != PastFlipIdx || CurSprite != PastSpriteIdx) {
					Pngidx++;
					PastFlipIdx = CurFlipbook;
					PastSpriteIdx = CurSprite;
				}
				if (Pngidx >= 10) {
					Pngidx = 0;
					vPos = Vec3(0.f, 40.f, -13.f);
				}

				// ���� ������ else if �� ��Ȯ�� �ϳ��� �����ǰ�!
				if (Pngidx <= 2) {
					Png_move = PNG_PLAYER_MOVE::RIGHTUP;
				}
				else if (Pngidx <= 5) {
					Png_move = PNG_PLAYER_MOVE::RIGHTDOWN;
				}
				else if (Pngidx <= 8) {
					Png_move = PNG_PLAYER_MOVE::LEFTUP;
				}
				else if (Pngidx <= 9) {
					Png_move = PNG_PLAYER_MOVE::LEFTDOWN;
				}

				if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
					vPos.y += m_Speed * DT * Move;
				}
				else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
					vPos.y -= m_Speed * DT * Move;
				}
				else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
					vPos.y += m_Speed * DT * Move;
				}
				else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
					vPos.y -= m_Speed * DT * Move * Three;
				}
			}
		}
		if (ObjectName == L"pLeftHand") {

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			CLayer* pLayer = pLevel->GetLayer(5);
			const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

			CGameObject* pBody = nullptr;

			for (int j = 0; j < vecObj.size(); j++) {
				//objNameStr.c_str()�� std::string�� ���� ���۸� ��ȯ�ϴµ�, std::vector�� �߰��Ǵ� ���� �ƴ϶�, ���� �޸� �ּҸ� ����Ű�� �����͸� �߰��ϴ� ���̾�.

				//objNameStr�� for������ ��� ����̹Ƿ�, ItemList���� ��� ���� �ּҰ� �����.
				string objNameStr = ToString(vecObj[j]->GetName());  // wstring�� string���� ��ȯ

				if (vecObj[j]->GetName() == L"pBody") {
					pBody = vecObj[j];
					break;
				}
			}


			Ptr<CMaterial> pMt = m_PlayerObject->MeshRender()->GetMaterial();

			if (pBody->GetHavePart1() == PART::NONE_HAND) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0007.png"));
			}
			else if (pBody->GetHavePart1() == PART::GEAR) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0008.png"));
			}
			else if (pBody->GetHavePart1() == PART::CARBON) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0011.png"));
			}
			else if (pBody->GetHavePart1() == PART::COPPER) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0009.png"));
			}
			else if (pBody->GetHavePart1() == PART::MACHINED_CARBON) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0012.png"));
			}
			else if (pBody->GetHavePart1() == PART::MACHINED_COPPER) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0010.png"));
			}

			if (pBody->FlipbookRender()->GetDir() == 1) {
				vPos.z = -10;
			}
			if (pBody->FlipbookRender()->GetDir() == 0) {
				vPos.z = -75;
			}

			if (pBody != nullptr) {
				int CurFlipbook = pBody->FlipbookRender()->GetFlipbookIdx();
				int CurSprite = pBody->FlipbookRender()->GetSpriteIdx();
				int CurDir = pBody->FlipbookRender()->GetDir();

				// ���� �ν�: Flipbook/Sprite�� �ٲ�鸸 png �ε��� ����
				if (CurFlipbook != PastFlipIdx || CurSprite != PastSpriteIdx) {
					if (CurFlipbook != PastFlipIdx) {
						m_PlayerObject->Transform()->SetRelativePos(20.f, -8.f, vPos.z);
					}
					Pngidx++;
					PastFlipIdx = CurFlipbook;
					PastSpriteIdx = CurSprite;
					PastDir = CurDir;
				}
				if (Pngidx >= 10) {
					Pngidx = 0;
					vPos = Vec3(20.f, -6.f, -11.f);
				}


				// ���� ������ else if �� ��Ȯ�� �ϳ��� �����ǰ�!
				if (Pngidx <= 2) {
					Png_move = PNG_PLAYER_MOVE::RIGHTUP;
				}
				else if (Pngidx <= 5) {
					Png_move = PNG_PLAYER_MOVE::RIGHTDOWN;
				}
				else if (Pngidx <= 8) {
					Png_move = PNG_PLAYER_MOVE::LEFTUP;
				}
				else if (Pngidx <= 9) {
					Png_move = PNG_PLAYER_MOVE::LEFTDOWN;
				}

				if (CurFlipbook == PLAYER_STATE::PLAYER_IDLE) {
					if (PastDir == 0) {
						if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
							//vPos.x += m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
							//vPos.x += m_Speed * DT * Move;
							vPos.y -= m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
							//vPos.x -= m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
							//vPos.x -= m_Speed * DT * Move * Three;
							vPos.y -= m_Speed * DT * Move * Three;
						}
					}
					if (PastDir == 1) {
						if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
							//vPos.x -= m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
							//vPos.x -= m_Speed * DT * Move;
							vPos.y -= m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
							//vPos.x += m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
							//vPos.x += m_Speed * DT * Move * Three;
							vPos.y -= m_Speed * DT * Move * Three;
						}
					}

				}
				if (CurFlipbook == PLAYER_STATE::PLAYER_MOVE) {
					if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
						vPos.y += m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
						vPos.y -= m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
						vPos.y += m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
						vPos.y -= m_Speed * DT * Move * Three;
					}
				}
			}

		}
		if (ObjectName == L"pRightHand") {

			Ptr<CMaterial> pMt = m_PlayerObject->MeshRender()->GetMaterial();

			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

			CLayer* pLayer = pLevel->GetLayer(5);
			const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

			CGameObject* pBody = nullptr;

			for (int j = 0; j < vecObj.size(); j++) {
				//objNameStr.c_str()�� std::string�� ���� ���۸� ��ȯ�ϴµ�, std::vector�� �߰��Ǵ� ���� �ƴ϶�, ���� �޸� �ּҸ� ����Ű�� �����͸� �߰��ϴ� ���̾�.

				//objNameStr�� for������ ��� ����̹Ƿ�, ItemList���� ��� ���� �ּҰ� �����.
				string objNameStr = ToString(vecObj[j]->GetName());  // wstring�� string���� ��ȯ

				if (vecObj[j]->GetName() == L"pBody") {
					pBody = vecObj[j];
					break;
				}
			}

			if (pBody->GetHavePart2() == PART::NONE_HAND) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0001.png"));
			}
			else if (pBody->GetHavePart2() == PART::GEAR) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0002.png"));
			}
			else if (pBody->GetHavePart2() == PART::CARBON) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0005.png"));
			}
			else if (pBody->GetHavePart2() == PART::COPPER) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0003.png"));
			}
			else if (pBody->GetHavePart2() == PART::MACHINED_CARBON) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0006.png"));
			}
			else if (pBody->GetHavePart2() == PART::MACHINED_COPPER) {
				pMt->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"Texture\\charaGGHand0004.png"));
			}

			if (pBody->FlipbookRender()->GetDir() == 1) {
				vPos.z = -75;
				//vPos.z = 0;
			}
			else if (pBody->FlipbookRender()->GetDir() == 0) {
				vPos.z = -10;
				//vPos.z = 0;
			}

			if (pBody != nullptr) {
				int CurFlipbook = pBody->FlipbookRender()->GetFlipbookIdx();
				int CurSprite = pBody->FlipbookRender()->GetSpriteIdx();
				int CurDir = pBody->FlipbookRender()->GetDir();

				// ���� �ν�: Flipbook/Sprite�� �ٲ�鸸 png �ε��� ����
				if (CurFlipbook != PastFlipIdx || CurSprite != PastSpriteIdx) {
					if (CurFlipbook != PastFlipIdx || PastDir != CurDir) {
						m_PlayerObject->Transform()->SetRelativePos(-20.f, -8.f, vPos.z);
					}
					Pngidx++;
					PastFlipIdx = CurFlipbook;
					PastSpriteIdx = CurSprite;
					PastDir = CurDir;

				}
				if (Pngidx >= 10) {
					Pngidx = 0;
					vPos = Vec3(-20.f, -8.f, -9.f);
				}

				// ���� ������ else if �� ��Ȯ�� �ϳ��� �����ǰ�!
				if (Pngidx <= 2) {
					Png_move = PNG_PLAYER_MOVE::RIGHTUP;
				}
				else if (Pngidx <= 5) {
					Png_move = PNG_PLAYER_MOVE::RIGHTDOWN;
				}
				else if (Pngidx <= 8) {
					Png_move = PNG_PLAYER_MOVE::LEFTUP;
				}
				else if (Pngidx <= 9) {
					Png_move = PNG_PLAYER_MOVE::LEFTDOWN;
				}

				if (CurFlipbook == PLAYER_STATE::PLAYER_IDLE) {
					if (PastDir == 0) {
						if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
							//vPos.x += m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
							//vPos.x += m_Speed * DT * Move;
							vPos.y -= m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
							//vPos.x -= m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
							//vPos.x -= m_Speed * DT * Move * Three;
							vPos.y -= m_Speed * DT * Move * Three;
						}
					}
					if (PastDir == 1) {
						if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
							//vPos.x -= m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
							//vPos.x -= m_Speed * DT * Move;
							vPos.y -= m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
							//vPos.x += m_Speed * DT * Move;
							vPos.y += m_Speed * DT * Move;
						}
						else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
							//vPos.x += m_Speed * DT * Move * Three;
							vPos.y -= m_Speed * DT * Move * Three;
						}
					}
				}
				if (CurFlipbook == PLAYER_STATE::PLAYER_MOVE) {
					if (Png_move == PNG_PLAYER_MOVE::RIGHTUP) { //0 1 2
						vPos.y += m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::RIGHTDOWN) { //3 4 5
						vPos.y -= m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::LEFTUP) { //6 7 8
						vPos.y += m_Speed * DT * Move;
					}
					else if (Png_move == PNG_PLAYER_MOVE::LEFTDOWN) { // 9
						vPos.y -= m_Speed * DT * Move * Three;
					}
				}
			}

		}

		pTrans->SetRelativePos(vPos);
	}

}
//bBlockCount += 1;
void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m = this->GetOwner();

	if (_Other->GetName() == L"pWall") {

		Vec2 playerColScale = this->Collider2D()->GetScale();
		Vec3 playerPos = this->Transform()->GetRelativePos();

		Vec2 wallColScale = _Other->Collider2D()->GetScale();
		Vec3 wallPos = _Other->Transform()->GetRelativePos();

		// �߽� ��ǥ �Ÿ�
		Vec2 delta = Vec2(playerPos.x - wallPos.x, playerPos.y - wallPos.y);
		// �� �ະ �浹 ��� 
		float overlapX = (playerColScale.x / 2 + wallColScale.x / 2) - abs(delta.x);
		float overlapY = (playerColScale.y / 2 + wallColScale.y / 2) - abs(delta.y);

		// X�� �浹
		if (overlapX < overlapY) {
			if (delta.x < 0) {
				// ���� ������
				m_bBlockRight = true;	
			}
			else {
				// ���� ����
				m_bBlockLeft = true;	
			}
		}
		// Y�� �浹
		else {
			if (delta.y < 0) {
				// ���� �Ʒ���
				m_bBlockUp = true;		
			}
			else {
				// ���� ����
				m_bBlockDown = true;    
			}
		}
	}

	//Player���� 
	if (_Other->GetName() == L"BigRhino" ||
		_Other->GetName() == L"SmallRhino" ||
		_Other->GetName() == L"Scrap" ||
		_Other->GetName() == L"SkulLeft" ||
		_Other->GetName() == L"SkulRight" ||
		_Other->GetName() == L"Zmora")
	{
		//���� Heat�� �����Ǿ��� ���
		if (m->GetPlayerHeat() == true && Invincibility == false) {
			//���� ���� �÷��̾��� ����� �������� �ʴ´ٸ�
			if (PlayerHP_Drone == false && CUR_PLAYER_STATE != PLAYER_STATE::PLAYER_DASH) {
				//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_death_arm_01 [1].wav");
				//pSound->Play(1, 0.2f, false);
				////���� Player ����� ���� ������ ��� �÷��̾� ���
				//CUR_PLAYER_STATE = PLAYER_STATE::PLAYER_DEATH;
				//CGameObject* pCharaDeath = new CGameObject;
				//pCharaDeath->SetName(L"pCharaDeath");
				//pCharaDeath->AddComponent(new CTransform);
				//pCharaDeath->AddComponent(new CFlipbookRender);
				//Vec3 PlayerPos = m->Transform()->GetRelativePos();
				//PlayerPos.z -= 1;
				//pCharaDeath->Transform()->SetRelativePos(PlayerPos);
				//pCharaDeath->Transform()->SetRelativeScale(880.f, 600.f, 1.f);

				//pCharaDeath->Transform()->SetIndependentScale(true);
				//pCharaDeath->FlipbookRender()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\CharaDeath.flip"));

				//pCharaDeath->FlipbookRender()->Play(0, 10, true);
				//SpawnGameObject(pCharaDeath, 5);


			}
			else {
				m->SetPlayerHeat(false);
				PlayerHP_Drone = false;
				Drone_CoolTime = 0.f;
				Invincibility = true;

				Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\char_player_damage_01 [1].wav");
				pSound->Play(1, 0.2f, false);
			}
		}
	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	wstring OtherName = _Other->GetName();
	if (OtherName == L"Pro_Aparatus" && KEY_TAP(ENTER)) {
		int a = 0;
	}
	else if (

		OtherName == L"Gear_Aparatus" && KEY_TAP(ENTER) ||
		OtherName == L"Copper_Aparatus" && KEY_TAP(ENTER) ||
		OtherName == L"Carbon_Aparatus" && KEY_TAP(ENTER) ||
		OtherName == L"Machined_Carbon_Aparatus" && KEY_TAP(ENTER) ||
		OtherName == L"Machined_Copper_Aparatus" && KEY_TAP(ENTER)
		)
	{

		//End(Part�������) -> Ready(Part�ȵ������) -> Start(����Ÿ��鼭 ����� ����) ->Ing(����� ��)
		APPARATUS_STATE a = _Other->GetAppState();

		//ó������ End�� Part ����ִ� ����
		CGameObject* m_CurObject = this->GetOwner();
		if (_Other->GetAppState() == APPARATUS_STATE::APPARATUS_END) {
			//_Other->SetAppState(APPARATUS_STATE::APPARATUS_READY);
			_Other->SetTag(true);


			if (OtherName == L"Gear_Aparatus") {
				// ###############################
				//			�� ���� ������
				// ###############################
				//�켱 ���� : �޼� -> ������
				//�޼տ� �ƹ��͵� ���� ���� �ʴ� �����̸� �޼տ� �켱������ Part�� ����
				if (m_CurObject->GetHavePart1() == PART::NONE_HAND) {
					m_CurObject->SetHavePart1(PART::GEAR);
					m_CurObject->SetRecentHand(1);
				}
				//�����տ� �ƹ��͵� ���� ���� �ʴ� �����̸� Part�� ����
				else if (m_CurObject->GetHavePart2() == PART::NONE_HAND) {
					m_CurObject->SetHavePart2(PART::GEAR);
					m_CurObject->SetRecentHand(2);
				}
				// ##################################
				//			�� ���� ���� ��
				// ##################################
				//�ֱٿ� Part�� ������ �޼�(1)�̸� ������(2)�� Part�� ����
				else if (m_CurObject->GetRecentHand() == 1) {
					m_CurObject->SetHavePart2(PART::GEAR);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ ������(2)�̸� �޼�(1)�� Part�� ����
				else if (m_CurObject->GetRecentHand() == 2) {
					m_CurObject->SetHavePart1(PART::GEAR);
					m_CurObject->SetRecentHand(1);
				}
			}
			else if (OtherName == L"Copper_Aparatus") {
				int a = m_CurObject->GetRecentHand();
				if (m_CurObject->GetHavePart1() == PART::NONE_HAND) {
					m_CurObject->SetHavePart1(PART::COPPER);
					m_CurObject->SetRecentHand(1);
				}
				else if (m_CurObject->GetHavePart2() == PART::NONE_HAND) {
					m_CurObject->SetHavePart2(PART::COPPER);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 1�̸� 2�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 1) {
					m_CurObject->SetHavePart2(PART::COPPER);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 2�̸� 1�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 2) {
					m_CurObject->SetHavePart1(PART::COPPER);
					m_CurObject->SetRecentHand(1);
				}
			}
			else if (OtherName == L"Carbon_Aparatus") {
				int a = m_CurObject->GetRecentHand();
				if (m_CurObject->GetHavePart1() == PART::NONE_HAND) {
					m_CurObject->SetHavePart1(PART::CARBON);
					m_CurObject->SetRecentHand(1);
				}
				else if (m_CurObject->GetHavePart2() == PART::NONE_HAND) {
					m_CurObject->SetHavePart2(PART::CARBON);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 1�̸� 2�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 1) {
					m_CurObject->SetHavePart2(PART::CARBON);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 2�̸� 1�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 2) {
					m_CurObject->SetHavePart1(PART::CARBON);
					m_CurObject->SetRecentHand(1);
				}

			}
			else if (OtherName == L"Machined_Carbon_Aparatus") {
				int a = m_CurObject->GetRecentHand();
				if (m_CurObject->GetHavePart1() == PART::NONE_HAND) {
					m_CurObject->SetHavePart1(PART::MACHINED_CARBON);
					m_CurObject->SetRecentHand(1);
				}
				else if (m_CurObject->GetHavePart2() == PART::NONE_HAND) {
					m_CurObject->SetHavePart2(PART::MACHINED_CARBON);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 1�̸� 2�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 1) {
					m_CurObject->SetHavePart2(PART::MACHINED_CARBON);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 2�̸� 1�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 2) {
					m_CurObject->SetHavePart1(PART::MACHINED_CARBON);
					m_CurObject->SetRecentHand(1);
				}

			}
			else if (OtherName == L"Machined_Copper_Aparatus") {
				int a = m_CurObject->GetRecentHand();
				if (m_CurObject->GetHavePart1() == PART::NONE_HAND) {
					m_CurObject->SetHavePart1(PART::MACHINED_COPPER);
					m_CurObject->SetRecentHand(1);
				}
				else if (m_CurObject->GetHavePart2() == PART::NONE_HAND) {
					m_CurObject->SetHavePart2(PART::MACHINED_COPPER);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 1�̸� 2�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 1) {
					m_CurObject->SetHavePart2(PART::MACHINED_COPPER);
					m_CurObject->SetRecentHand(2);
				}
				//�ֱٿ� Part�� ������ 2�̸� 1�� �տ� Gear�� ����
				else if (m_CurObject->GetRecentHand() == 2) {
					m_CurObject->SetHavePart1(PART::MACHINED_COPPER);
					m_CurObject->SetRecentHand(1);
				}

			}
		}
		// Reday Part ������� �ʴ� ����
		else if (_Other->GetAppState() == APPARATUS_STATE::APPARATUS_READY) {
			//_Other->SetAppState(APPARATUS_STATE::APPARATUS_START);
			//_Other->SetAppState(APPARATUS_STATE::APPARATUS_END);
			_Other->SetTag(true);
		}
	}
	else if ((OtherName == L"Generator_0" ||
		OtherName == L"Generator_1" ||
		OtherName == L"Generator_2" ||
		OtherName == L"Generator_3")

		&& KEY_TAP(ENTER)) {
		CGameObject* m_CurObject = this->GetOwner();
		_Other->SetTag(true);
		//�����Ⱑ ����ǥ �����϶� �ǵ�� ���ϴ� ������ ������
		if (_Other->GetGenerator_ui_state() == GENERATOR_UI_STATE::GENERATOR_UI_QUESTION) {
			_Other->SetGenerator_ui_state(GENERATOR_UI_PART);
			_Other->SetUiSpawnPart(false);
			_Other->SetGeneratorUIUpdate(true);
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Sound\\obj_generator_part_reweal_01_L [1].wav");
			pSound->Play(1, 0.2f, false);
		}
	}

}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m = this->GetOwner();

	if (m->GetName() == L"pBody") {
		wstring OtherName = _Other->GetName();
		if (OtherName == L"pWall") {
			bBlockCount -= 1;
			if (m_PlayerLRDir == MOVE_LEFT) {
				m_bBlockRight = false;
			}
			if (m_PlayerLRDir == MOVE_RIGHT) {
				m_bBlockLeft = false;
			}
			if (m_PlayerUDDir == MOVE_UP) {
				m_bBlockDown = false;
			}
			if (m_PlayerUDDir == MOVE_DOWN) {
				m_bBlockUp = false;
			}
			m_bBlockLeft = false;
			m_bBlockRight = false;
			m_bBlockUp = false;
			m_bBlockDown = false;
		}
	}
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	fwrite(&m_PaperBurn, sizeof(float), 1, _File);
	SaveAssetRef(m_Prefab, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_PaperBurn, sizeof(float), 1, _File);
	m_Prefab = LoadAssetRef<CPrefab>(_File);
}
