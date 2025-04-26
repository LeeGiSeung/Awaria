#include "pch.h"
#include "CParticleRender.h"

#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"
#include "CTimeMgr.h"

#include "CKeyMgr.h"

CParticleRender::CParticleRender()
	: CRenderComponent(COMPONENT_TYPE::PARTICLERENDER)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticle(1000)
	, m_Module{}
	, m_AccTime(0.f)
{
	m_SpawnBuffer = new CStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_ONLY, true);

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false);

	CreateMtrl();

	// 파티클 Tick 컴퓨트 쉐이더
	Ptr<CComputeShader> pCS = CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS");

	if (nullptr == pCS)
	{
		pCS = new CParticleTickCS;
		pCS->SetName(L"ParticleTickCS");
		CAssetMgr::GetInst()->AddAsset(pCS->GetName(), pCS.Get());
	}

	m_TickCS = (CParticleTickCS*)pCS.Get();
}

CParticleRender::CParticleRender(const CParticleRender& _Origin)
	: CRenderComponent(_Origin)
	, m_ParticleBuffer(nullptr)
	, m_SpawnBuffer(nullptr)
	, m_ModuleBuffer(nullptr)	
	, m_TickCS(_Origin.m_TickCS)
	, m_MaxParticle(_Origin.m_MaxParticle)
	, m_Module(_Origin.m_Module)
	, m_ModuleChanged(false)
	, m_AccTime(0.f)
	, m_ParticleTex(_Origin.m_ParticleTex)
{
	m_SpawnBuffer = new CStructuredBuffer;
	m_SpawnBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true);

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_ONLY, true);

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticle, SB_TYPE::SRV_UAV, false);

	if (nullptr == m_TickCS)
	{
		m_TickCS = new CParticleTickCS;
		m_TickCS->SetName(L"ParticleTickCS");
		CAssetMgr::GetInst()->AddAsset(m_TickCS->GetName(), m_TickCS.Get());
	}	
}

CParticleRender::~CParticleRender()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_SpawnBuffer)
		delete m_SpawnBuffer;

	if (nullptr != m_ModuleBuffer)
		delete m_ModuleBuffer;
}

void CParticleRender::FinalTick()
{
	m_ModuleChanged = true;
	m_Module.ObjectWorldPos = Transform()->GetWorldPos();

	// 이번 프레임 파티클 활성화 개수 계산
	CalcSpawnCount();

	// 파티클 모듈 변경점 반영
	if (m_ModuleChanged)
	{
		m_ModuleChanged = false;
		m_ModuleBuffer->SetData(&m_Module);
	}

	m_TickCS->SetSpawnBuffer(m_SpawnBuffer);
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->SetNoiseTex(CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));

	if (KEY_PRESSED(KEY::RBTN))
		m_TickCS->SetStop(true);
	else
		m_TickCS->SetStop(false);
	
	m_TickCS->Excute();
}

void CParticleRender::Render()
{
	Transform()->Binding();

	// 파티클 버퍼 바인딩
	m_ParticleBuffer->Binding(20);

	// 모듈 버퍼 바인딩
	m_ModuleBuffer->Binding(21);

	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->SetScalarParam(VEC4_0, Transform()->GetWorldPos());

	// 재질 및 쉐이더 바인딩
	GetMaterial()->Binding();

	// 파티클 인스턴싱 렌더링
	GetMesh()->Render_Particle(m_MaxParticle);

	// 파티클 버퍼 Clear
	m_ParticleBuffer->Clear(20);
	// 모듈버퍼 Clear
	m_ModuleBuffer->Clear(21);
}


void CParticleRender::CreateMtrl()
{	
	if (nullptr == GetMesh())
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));

	if (nullptr != GetMaterial())
		return;

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleMtrl");
	if (nullptr != pMtrl)
	{
		SetMaterial(pMtrl);
		return;
	}

	// 파티클 렌더링 쉐이더 및 재질
	Ptr<CGraphicShader> pShader = new CGraphicShader;
	pShader->SetName(L"ParticleRenderShader");

	pShader->CreateVertexShader(L"HLSL\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"HLSL\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"HLSL\\particle.fx", "PS_Particle");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CAssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	pMtrl = new CMaterial;
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(pShader);
	CAssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	
	// 머티리얼 설정
	SetMaterial(pMtrl);
}

void CParticleRender::CalcSpawnCount()
{
	m_AccTime += EngineDT;
	float Term = 1.f / m_Module.SpawnRate;

	tSpawnCount count = {};

	if (Term < m_AccTime)
	{
		m_AccTime -= Term;		
		count.SpawnCount = 1;		
	}

	if (m_Module.Module[(int)PARTICLE_MODULE::SPAWN_BURST] && 0 < m_Module.SpawnBurstRepeat)
	{
		static float AccTime = 0.f;
		AccTime += EngineDT;

		if (m_Module.SpawnBurstRepeatTime < AccTime)
		{
			count.SpawnCount += m_Module.SpawnBurstCount;
			
			m_Module.SpawnBurstRepeat -= 1;
			AccTime -= m_Module.SpawnBurstRepeatTime;
		}
	}

	if (0 < count.SpawnCount)
	{
		m_SpawnBuffer->SetData(&count);
	}
}


void CParticleRender::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_TickCS, _File);
	SaveAssetRef(m_ParticleTex, _File);
	fwrite(&m_MaxParticle, sizeof(UINT), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);

	
}

void CParticleRender::LoadFromFile(FILE* _File)
{
	m_TickCS = (CParticleTickCS*)LoadAssetRef<CComputeShader>(_File).Get();
	m_ParticleTex = LoadAssetRef<CTexture>(_File);
	fread(&m_MaxParticle, sizeof(UINT), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}
