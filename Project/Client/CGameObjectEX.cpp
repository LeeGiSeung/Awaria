#include "pch.h"
#include "CGameObjectEX.h"

#include <Engine/CComponent.h>

CGameObjectEX::CGameObjectEX()
{

}

CGameObjectEX::~CGameObjectEX()
{

}

void CGameObjectEX::FinalTick()
{
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr != GetComponent((COMPONENT_TYPE)i))
			GetComponent((COMPONENT_TYPE)i)->FinalTick();
	}
	
	vector<CGameObject*>::const_iterator iter = GetChild().begin();
	for (; iter != GetChild().end(); ++iter)
	{
		(*iter)->FinalTick();	
	}
}
