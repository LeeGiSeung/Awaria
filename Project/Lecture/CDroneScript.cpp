#include "pch.h"
#include "CDroneScript.h"

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

CDroneScript::CDroneScript()
	: CScript(DRONESCRIPT)
{
}

CDroneScript::~CDroneScript()
{
}

void CDroneScript::Init()
{
}

void CDroneScript::Begin()
{
}

void CDroneScript::Tick()
{
	CGameObject* m_CurObject = this->GetOwner();

	Vec3 TargetPos;
	int TargetDir = 0;
	bool PlayerHit = false;
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(5); //PlayerLayer = 5
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	for (int j = 0; j < vecObj.size(); j++) {
		if (vecObj[j]->GetName() == L"pBody") {
			TargetPos = vecObj[j]->Transform()->GetRelativePos();
			TargetDir = vecObj[j]->FlipbookRender()->GetDir();
			PlayerHit = vecObj[j]->GetPlayerHeat();
		}
	}

	//if (this->GetOwner()->GetPlayerHeat() && m_Drone_State != DRONE_STATE::DRONE_DEATH) {
	//	m_Drone_State = DRONE_STATE::DRONE_DEATH;
	//	m_CurObject->FlipbookRender()->Play(DRONE_STATE::DRONE_DEATH, 8, false);
	//}
	//else if (FlipbookRender()->IsFinish() && m_Drone_State == DRONE_STATE::DRONE_DEATH) {
	//	this->GetOwner()->Destroy();
	//	return;
	//}

	//Player가 왼쪽으로 달리고 있으면
	if (TargetDir == 0) {
		TargetPos.y += 100;
		TargetPos.x += 80;
		FlipbookRender()->SetDir(0);
	}
	else {
		TargetPos.y += 100;
		TargetPos.x -= 80;
		FlipbookRender()->SetDir(1);
	}

	Transform()->SetRelativePos(TargetPos);

}

void CDroneScript::SaveToFile(FILE* _File)
{
}

void CDroneScript::LoadFromFile(FILE* _File)
{
}
