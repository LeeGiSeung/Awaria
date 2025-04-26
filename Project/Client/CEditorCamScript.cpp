#include "pch.h"
#include "CEditorCamScript.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>

CEditorCamScript::CEditorCamScript()
	: CScript(-1)
	, m_CamSpeed(100.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::Tick()
{
	// 카메라 컴포넌트가 없으면 종료
	if (!Camera())
		return;

	PROJ_TYPE type = Camera()->GetProjType();

	if (PROJ_TYPE::PERSPECTIVE == type)
		Move_Perspective();
	else
		Move_OrthoGraphic();
}

void CEditorCamScript::Move_Perspective()
{
	Vec3 vPos = Transform()->GetRelativePos();

	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

	if (KEY_PRESSED(KEY::UP))
		vPos += vFront * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::DOWN))
		vPos -= vFront * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::LEFT))
		vPos -= vRight * EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::RIGHT))
		vPos += vRight * EngineDT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);


	if (!KEY_PRESSED(KEY::RBTN))
		return;

	Vec2 vDragDir = CKeyMgr::GetInst()->GetDragDir();
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.x += vDragDir.y * EngineDT * 1800.f;
	vRot.y += vDragDir.x * EngineDT * 1800.f;
	Transform()->SetRelativeRotation(vRot);
}

void CEditorCamScript::Move_OrthoGraphic()
{
	Vec3 vRot = Vec3(0.f, 0.f, 0.f);
	Transform()->SetRelativeRotation(vRot);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
		vPos.y += EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= EngineDT * m_CamSpeed;
	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += EngineDT * m_CamSpeed;

	Transform()->SetRelativePos(vPos);
}