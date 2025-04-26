#include "pch.h"
#include "CSpriteRender.h"

#include "CAssetMgr.h"
#include "CTransform.h"

CSpriteRender::CSpriteRender()
	: CRenderComponent(COMPONENT_TYPE::SPRITERENDER)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateSpriteMaterial();
}

CSpriteRender::~CSpriteRender()
{
}


void CSpriteRender::FinalTick()
{
}

void CSpriteRender::Render()
{
	// 위치정보 업데이트
	Transform()->Binding();

	// 사용할 쉐이더 바인딩	
	if (nullptr != m_Sprite)
	{
		GetMaterial()->SetScalarParam(INT_0, (int)!!m_Sprite->GetAtlasTex().Get());
		GetMaterial()->SetTexParam(TEX_0, m_Sprite->GetAtlasTex());
		GetMaterial()->SetScalarParam(VEC2_0, m_Sprite->GetLeftTop());
		GetMaterial()->SetScalarParam(VEC2_1, m_Sprite->GetSlice());
	}
	else
	{
		GetMaterial()->SetScalarParam(INT_0, 0);
		GetMaterial()->SetTexParam(TEX_0, nullptr);
	}

	GetMaterial()->Binding();

	// 렌더링 시작
	GetMesh()->Render();
}


void CSpriteRender::CreateSpriteMaterial()
{
	if (nullptr == CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"SpriteShader"))
	{
		// SpriteShader
		Ptr<CGraphicShader> pShader = new CGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\sprite.fx", "VS_Sprite");
		pShader->CreatePixelShader(L"HLSL\\sprite.fx", "PS_Sprite");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		CAssetMgr::GetInst()->AddAsset(L"SpriteShader", pShader.Get());
	}

	if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpriteMtrl"))
	{
		// SpriteMtrl
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"SpriteShader"));
		CAssetMgr::GetInst()->AddAsset(L"SpriteMtrl", pMtrl.Get());
	}

	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpriteMtrl"));
}


void CSpriteRender::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	SaveAssetRef(m_Sprite, _File);
}

void CSpriteRender::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	m_Sprite = LoadAssetRef<CSprite>(_File);
}