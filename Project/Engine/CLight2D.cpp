#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"


CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_Info{}
{
	m_Info.Type = LIGHT_TYPE::DIRECTIONAL;
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.Dir = Transform()->GetWorldDir(DIR::RIGHT);
	
	CRenderMgr::GetInst()->RegisterLight2D(this);

	if (m_Info.Type != LIGHT_TYPE::DIRECTIONAL)
	{
		Vec3 vWorldPos = Transform()->GetWorldPos();
		DrawDebugRect(Vec4(0.f, 1.f, 0.f, 1.f), vWorldPos, Vec3(20.f, 20.f, 1.f), Vec3(0.f, 0.f, 0.f), true);
	}	
}

void CLight2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLight2DInfo), 1, _File);
}

void CLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLight2DInfo), 1, _File);
}
