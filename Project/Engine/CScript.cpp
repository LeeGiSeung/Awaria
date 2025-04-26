#include "pch.h"
#include "CScript.h"

#include "CTransform.h"

CScript::CScript(int _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::CScript(const CScript& _Origin)
	: CComponent(_Origin)
	, m_ScriptType(_Origin.m_ScriptType)	
{
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _Prefab, int _LayerIdx, Vec3 _WorldPos)
{
	assert(_Prefab.Get());

	CGameObject* pNewObject = _Prefab->Instantiate();
	pNewObject->Transform()->SetRelativePos(_WorldPos);
	SpawnGameObject(pNewObject, _LayerIdx);
}

