#include "pch.h"
#include "CScriptMgr.h"

#include "CAparatusScript.h"
#include "CBigRhinoScript.h"
#include "CBombSkulScript.h"
#include "CBossVectorScript.h"
#include "CCameraMoveScript.h"
#include "CCutSceneScript.h"
#include "CDroneScript.h"
#include "CGeneratorScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"
#include "CSkulScript.h"
#include "CSmallRhinoScript.h"
#include "CZmoraScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAparatusScript");
	_vec.push_back(L"CBigRhinoScript");
	_vec.push_back(L"CBombSkulScript");
	_vec.push_back(L"CBossVectorScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCutSceneScript");
	_vec.push_back(L"CDroneScript");
	_vec.push_back(L"CGeneratorScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSkulScript");
	_vec.push_back(L"CSmallRhinoScript");
	_vec.push_back(L"CZmoraScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAparatusScript" == _strScriptName)
		return new CAparatusScript;
	if (L"CBigRhinoScript" == _strScriptName)
		return new CBigRhinoScript;
	if (L"CBombSkulScript" == _strScriptName)
		return new CBombSkulScript;
	if (L"CBossVectorScript" == _strScriptName)
		return new CBossVectorScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCutSceneScript" == _strScriptName)
		return new CCutSceneScript;
	if (L"CDroneScript" == _strScriptName)
		return new CDroneScript;
	if (L"CGeneratorScript" == _strScriptName)
		return new CGeneratorScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSkulScript" == _strScriptName)
		return new CSkulScript;
	if (L"CSmallRhinoScript" == _strScriptName)
		return new CSmallRhinoScript;
	if (L"CZmoraScript" == _strScriptName)
		return new CZmoraScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::APARATUSSCRIPT:
		return new CAparatusScript;
		break;
	case (UINT)SCRIPT_TYPE::BIGRHINOSCRIPT:
		return new CBigRhinoScript;
		break;
	case (UINT)SCRIPT_TYPE::BOMBSKULSCRIPT:
		return new CBombSkulScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSVECTORSCRIPT:
		return new CBossVectorScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CUTSCENESCRIPT:
		return new CCutSceneScript;
		break;
	case (UINT)SCRIPT_TYPE::DRONESCRIPT:
		return new CDroneScript;
		break;
	case (UINT)SCRIPT_TYPE::GENERATORSCRIPT:
		return new CGeneratorScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SKULSCRIPT:
		return new CSkulScript;
		break;
	case (UINT)SCRIPT_TYPE::SMALLRHINOSCRIPT:
		return new CSmallRhinoScript;
		break;
	case (UINT)SCRIPT_TYPE::ZMORASCRIPT:
		return new CZmoraScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::APARATUSSCRIPT:
		return L"CAparatusScript";
		break;

	case SCRIPT_TYPE::BIGRHINOSCRIPT:
		return L"CBigRhinoScript";
		break;

	case SCRIPT_TYPE::BOMBSKULSCRIPT:
		return L"CBombSkulScript";
		break;

	case SCRIPT_TYPE::BOSSVECTORSCRIPT:
		return L"CBossVectorScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CUTSCENESCRIPT:
		return L"CCutSceneScript";
		break;

	case SCRIPT_TYPE::DRONESCRIPT:
		return L"CDroneScript";
		break;

	case SCRIPT_TYPE::GENERATORSCRIPT:
		return L"CGeneratorScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SKULSCRIPT:
		return L"CSkulScript";
		break;

	case SCRIPT_TYPE::SMALLRHINOSCRIPT:
		return L"CSmallRhinoScript";
		break;

	case SCRIPT_TYPE::ZMORASCRIPT:
		return L"CZmoraScript";
		break;

	}
	return nullptr;
}