#include "pch.h"
#include "CCutSceneScript.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CCollider2D.h>
#include <Engine/CMeshRender.h>

CCutSceneScript::CCutSceneScript()
	:CScript(CUTSCENESCRIPT)
{

}

CCutSceneScript::~CCutSceneScript()
{
}

void CCutSceneScript::Init()
{
}

void CCutSceneScript::Begin()
{
}

void CCutSceneScript::Tick()
{
}

void CCutSceneScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
}

void CCutSceneScript::SaveToFile(FILE* _File)
{
}

void CCutSceneScript::LoadFromFile(FILE* _File)
{
}

