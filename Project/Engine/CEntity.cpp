#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_NextId = 0;

CEntity::CEntity()
	: m_ID(g_NextId++)
{
}

CEntity::CEntity(const CEntity& _Origin)
	: m_ID(g_NextId++)
	, m_Name(_Origin.m_Name)
{
}

CEntity::~CEntity()
{
}
