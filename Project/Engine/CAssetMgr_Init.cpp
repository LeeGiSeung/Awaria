#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"
#include "assets.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}

void CAssetMgr::CreateEngineMesh()
{
	// =========
	// PointMesh
	// =========
	Ptr<CMesh> pMesh = new CMesh(true);
	Vtx vPoint;
	vPoint.vPos = Vec3(0.f, 0.f, 0.f);
	vPoint.vUV = Vec2(0.f, 0.f);
	vPoint.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	UINT Idx = 0;
	pMesh->Create(&vPoint, 1, &Idx, 1);
	AddAsset(L"PointMesh", pMesh.Get());

	// ========
	// RectMesh
	// ========	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	pMesh = new CMesh(true);
	pMesh->SetName(L"RectMesh");

	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// Index
	UINT arrIdx[6] = { 0, 1, 2, 0, 2, 3 };

	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", pMesh.Get());


	// ==============
	// RectMesh_Debug
	// ==============	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	pMesh = new CMesh(true);
	pMesh->SetName(L"RectMesh_Debug");

	arrIdx[0] = 0; arrIdx[1] = 1; arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0; 
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh.Get());

	// ==========
	// CircleMesh
	// ==========
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;


	pMesh = new CMesh(true);
	pMesh->SetName(L"CircleMesh");

	// Circle 중심점
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	int Slice = 40;
	float Radius = 0.5f;
	float Angle = 0.f;

	// Circle 테두리 정점
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);		
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Angle += XM_2PI / (float)Slice;
	}

	// Circle 인덱스
	for(int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh.Get());	
	vecIdx.clear();

	// ================
	// CircleMesh_Debug
	// ================
	pMesh = new CMesh(true);
	pMesh->SetName(L"CircleMesh_Debug");

	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh.Get());
	vecVtx.clear();
	vecIdx.clear();
}

void CAssetMgr::CreateEngineTexture()
{
	Ptr<CTexture> pTex = new CTexture;
	wstring strPath = CPathMgr::GetContentPath();
	pTex->Load(strPath + L"Texture\\link.png");
	AddAsset(L"Link", pTex.Get());
}

void CAssetMgr::CreateEngineGraphicShader()
{
	// 표준 쉐이더 생성
	Ptr<CGraphicShader>	pShader = nullptr;

	// ===========
	// Std2DShader
	// ===========
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL\\std2d.fx", "PS_Std2D");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	// Parameter	
	pShader->AddScalarParam(INT_0, L"Test 1");
	pShader->AddScalarParam(INT_1, L"Test 2");

	pShader->AddTexParam(TEX_0, L"OutputTexture");

	AddAsset(L"Std2DShader", pShader.Get());


	// =====================
	// Std2DAlphaBlendShader
	// =====================
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	AddAsset(L"Std2DAlphaBlendShader", pShader.Get());


	// PostProcess 쉐이더
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"HLSL\\postprocess.fx", "PS_PostProcess");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WIRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	AddAsset(L"PostProcessShader", pShader.Get());


	// Distortion Shader
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"HLSL\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"HLSL\\postprocess.fx", "PS_Distortion");
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WIRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	AddAsset(L"DistortionShader", pShader.Get());
}

void CAssetMgr::CreateEngineComputeShader()
{
}

void CAssetMgr::CreateEngineMaterial()
{
	// Std2DMtrl
	Ptr<CMaterial> pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"PlayerHead", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"PlayerBody", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"PlayerLeftHand", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"PlayerRightHand", pMtrl.Get());

	// PartMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Machined_Copper", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Machined_Carbon", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Carbon", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Copper", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Gear", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"CarbonWantUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"CopperWantUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"GearWantUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Machined_CarbonWantUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Machined_CopperWantUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"NoneUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"QuestionUI", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"SuccessUI", pMtrl.Get());
	
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap0", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap1", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap2", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap3", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap4", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap5", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap6", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap7", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap8", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Scrap9", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"victoryScreen", pMtrl.Get());

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"BlackBackground", pMtrl.Get());

	// PostProcessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PostProcessShader"));
	AddAsset(L"PostProcessMtrl", pMtrl.Get());

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	AddAsset(L"DistortionMtrl", pMtrl.Get());

	pMtrl->SetTexParam(TEX_1, Load<CTexture>(L"Texture\\noise\\Noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));
}

