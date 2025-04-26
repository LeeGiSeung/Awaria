#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"
#include "CTimeMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCam(nullptr)	
	, m_DebugObject(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_RenderFunc(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;

	m_RenderFunc = &CRenderMgr::Render_Editor;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Priority)
{
	if (m_vecCam.size() <= _Priority)
	{
		m_vecCam.resize(_Priority + 1);
	}

	if (m_vecCam[_Priority] != nullptr)
	{
		m_vecCam[_Priority]->m_Priority = -1;
	}
	
	m_vecCam[_Priority] = _Cam;	
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");	

	// RenderTarget -> Copy -> PostProcessTex
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::Tick()
{
	// Resource GPU Binding
	Binding();
	 
	// Main Rendering	
	CDevice::GetInst()->ClearTarget();
	(this->*m_RenderFunc)();
		
	// Debug Rendering
	DebugRender();

	// Resource Clear
	Clear();
}

void CRenderMgr::Binding()
{
	// 출력 렌더타겟 및 출력 깊이타겟 설정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");		
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// Light2D Binding
	// 광원 개수에 맞춰서 구조화 버퍼 리사이즈
	if (m_Light2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
	}

	// 각 광원의 tLight2DInfo 구조체 정보를 벡터에 모음
	static vector<tLight2DInfo> vecLight2DInfo;
	vecLight2DInfo.clear();
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}	

	if (!vecLight2DInfo.empty())
	{
		// tLight2DInfo 정보를 모아서 구조화버퍼에 전달
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size());

		// t12 에 바인딩
		m_Light2DBuffer->Binding(12);
	}

	// GlobalData Binding
	static CConstBuffer* pGlobal = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);

	// 현재 레벨에 존재하는 광원의 개수 정보 업데이트
	g_global.g_Light2DCount = vecLight2DInfo.size();

	pGlobal->SetData(&g_global, sizeof(tGlobal));
	pGlobal->Binding();
	pGlobal->Binding_CS();
}

void CRenderMgr::Render_Play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->Render();
	}
}

void CRenderMgr::Render_Editor()
{
	if (nullptr == m_EditorCam)
		return;

	m_EditorCam->Render();
}

void CRenderMgr::Clear()
{
	m_Light2DBuffer->Clear(12);
	m_vecLight2D.clear();
}

void CRenderMgr::DebugRender()
{
	vector<tDebugShapeInfo>::iterator iter = m_vecDebugInfo.begin();
	for ( ; iter != m_vecDebugInfo.end(); )
	{		
		// 디버그 모양에 따른 메쉬 선택
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));			
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->GetRenderComponent()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:
			break;
		case DEBUG_SHAPE::CUBE:
			break;
		case DEBUG_SHAPE::SPHERE:
			break;
		default:
			break;
		}
			
		// 깊이 판정에 따라서 DepthStencilState 를 골라줌		
		if ((*iter).DepthTest)
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->GetRenderComponent()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WIRITE);

		// 색상값을 재질을 통해서 전달
		m_DebugObject->GetRenderComponent()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);
			
		// 위치정보 세팅 및 월드행렬 계산
		if ((*iter).MatWorld == Matrix::Identity)
		{
			m_DebugObject->Transform()->SetRelativePos((*iter).WorldPos);
			m_DebugObject->Transform()->SetRelativeScale((*iter).WorldScale);
			m_DebugObject->Transform()->SetRelativeRotation((*iter).WorldRotation);
			m_DebugObject->Transform()->FinalTick();
		}
		else
		{
			m_DebugObject->Transform()->SetWorldMat((*iter).MatWorld);
		}

		// 디버그 오브젝트 렌더링
		m_DebugObject->Render();

		// 노출 시간이 넘어선 정보는 제거한다.
		(*iter).CurTime += EngineDT;
		if ((*iter).Duration < (*iter).CurTime)
			iter = m_vecDebugInfo.erase(iter);
		else
			++iter;
	}
}