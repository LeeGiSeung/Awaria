#include "pch.h"
#include "CSetColorCS.h"



CSetColorCS::CSetColorCS()
	: CComputeShader(L"HLSL\\SetColor.fx", "CS_SetColor", 32, 32, 1)
{
}

CSetColorCS::~CSetColorCS()
{
}

int CSetColorCS::Binding()
{
	if (nullptr == m_TargetTex)
		return E_FAIL;

	m_TargetTex->Binding_CS_UAV(0);

	m_Const.v4Arr[0] = m_Color;
	m_Const.iArr[0] = (int)m_TargetTex->GetWidth();
	m_Const.iArr[1] = (int)m_TargetTex->GetHeight();

	return S_OK;
}

void CSetColorCS::CalcGroupNum()
{
	m_GroupX = m_TargetTex->GetWidth() / m_GroupPerThreadX;
	m_GroupY = m_TargetTex->GetHeight() / m_GroupPerThreadY;
	m_GroupZ = 1;

	if (m_TargetTex->GetWidth() % m_GroupPerThreadX)
		m_GroupX += 1;

	if (m_TargetTex->GetHeight() % m_GroupPerThreadY)
		m_GroupY += 1;
}

void CSetColorCS::Clear()
{
	m_TargetTex->Clear_CS_UAV();
}