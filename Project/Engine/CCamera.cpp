#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"
#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"
#include "CGraphicShader.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_OrthoScaleX(1.f)
	, m_AspectRatio(1.f)
	, m_FOV(XM_PI / 2.f)
	, m_Far(10000.f)
	, m_Priority(-1)
	, m_LayerCheck(0)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_OrthoScaleX = vResolution.x;
	m_AspectRatio = vResolution.x / vResolution.y;		
}

CCamera::CCamera(const CCamera& _Origin)
	: CComponent(_Origin)
	, m_ProjType(_Origin.m_ProjType)
	, m_OrthoScaleX(_Origin.m_OrthoScaleX)
	, m_AspectRatio(_Origin.m_AspectRatio)
	, m_FOV(_Origin.m_FOV)
	, m_Far(_Origin.m_Far)
	, m_Priority(_Origin.m_Priority)
	, m_LayerCheck(_Origin.m_LayerCheck)
{	
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);

	
}

void CCamera::FinalTick()
{
	// 뷰행렬 계산
	// 이동
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 회전
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
	matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
	matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// 투영행렬 계산하기
	if (ORTHOGRAPHIC == m_ProjType)
	{
		m_matProj = XMMatrixOrthographicLH(m_OrthoScaleX, m_OrthoScaleX / m_AspectRatio, 1.f, m_Far);
	}
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void CCamera::Render()
{
	// 카메라의 View, Proj 행렬을 세팅
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// 물체 분류
	SortObject();

	// 물체 렌더링
	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// PostProcess
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// 렌더타겟 장면을 PostProcess 타겟으로 복사
		CRenderMgr::GetInst()->CopyRenderTarget();

		// 복사된 PostProcess 텍스쳐를 t14(고정) 로 바인딩
		CRenderMgr::GetInst()->GetPostProcessTex()->Binding(14);

		m_vecPostProcess[i]->Render();
	}
}


void CCamera::SortObject()
{
	// 이전 분류작업 Clear
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecPostProcess.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Camera 가 Rendering 하지 않는 레이어는 거른다.	
		if (!( m_LayerCheck & (1 << i)))		
			continue;
	
		// 레이어에 속한 물체들을 가져온다.
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
				
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 레이어 안에있는 물체들 중에서 렌더링 기능이 없는 물체는 거른다.
			if (vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMaterial() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader() == nullptr)
				continue;

			// 오브젝트가 사용하는 쉐이더의 도메인에 따라서 분류한다.
			switch (vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader()->GetDomain())
			{
			case SHADER_DOMAIN::DOMAIN_QPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			}
		}
	}
}


void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_OrthoScaleX, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);	                   
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_OrthoScaleX, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
}
