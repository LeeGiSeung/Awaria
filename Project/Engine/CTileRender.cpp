#include "pch.h"
#include "CTileRender.h"

#include "CAssetMgr.h"
#include "CTransform.h"
#include "CStructuredBuffer.h"

CTileRender::CTileRender()
	: CRenderComponent(COMPONENT_TYPE::TILERENDER)
	, m_Col(0)
	, m_Row(0)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	CreateTileRenderMtrl();

	m_GpuBuffer = new CStructuredBuffer;
}

CTileRender::CTileRender(const CTileRender& _Origin)
	: CRenderComponent(_Origin)
	, m_Col(_Origin.m_Col)
	, m_Row(_Origin.m_Row)
	, m_TileSize(_Origin.m_TileSize)
	, m_GpuBuffer(nullptr)
{
	m_GpuBuffer = new CStructuredBuffer;
		
	// 벡터크기를 타일 개수만큼 재 설정
	m_vecTileInfo.resize(m_Col * m_Row);
	
	for (size_t i = 0; i < _Origin.m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i] = new tTileInfo(*_Origin.m_vecTileInfo[i]);
	}
}


CTileRender::~CTileRender()
{
	Delete_Vec(m_vecTileInfo);

	if (nullptr != m_GpuBuffer)
		delete m_GpuBuffer;
}

void CTileRender::Init()
{
	// 오브젝트 스케일 재 조정
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));

	// 타일 데이터를 버퍼로 전송
	UpdateBuffer();
}

void CTileRender::FinalTick()
{

}

void CTileRender::Render()
{
	// 위치정보 업데이트
	Transform()->Binding();
		
	GetMaterial()->SetScalarParam(INT_0, (int)!!m_vecTileInfo[0]->Sprite->GetAtlasTex().Get());
	GetMaterial()->SetScalarParam(INT_1, m_Col);
	GetMaterial()->SetScalarParam(INT_2, m_Row);

	GetMaterial()->SetTexParam(TEX_0, GetAtlasTex());
	GetMaterial()->SetScalarParam(VEC2_0, m_vecTileInfo[0]->Sprite->GetLeftTop());
	GetMaterial()->SetScalarParam(VEC2_1, m_vecTileInfo[0]->Sprite->GetSlice());

	GetMaterial()->Binding();

	// 타일정보를 저장하고있는 구조화버퍼 바인딩
	m_GpuBuffer->Binding(20);

	// 렌더링 시작
	GetMesh()->Render();
}

void CTileRender::SetColRow(UINT _Col, UINT _Row)
{
	m_Col = _Col;
	m_Row = _Row;

	if ((m_Col * m_Row) < m_vecTileInfo.capacity())
	{
		vector<tTileInfo*> temp;
		m_vecTileInfo.swap(temp);
		Delete_Vec(temp);
	}

	// 벡터크기를 타일 개수만큼 재 설정
	m_vecTileInfo.resize(m_Col * m_Row);

	// 각 포인터가 타일정보를 가질 수 있게 함
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i] = new tTileInfo;
	}

	// 오브젝트 스케일 재 조정
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));

	// 타일 데이터를 버퍼로 전송
	UpdateBuffer();
}

void CTileRender::SetTileSize(Vec2 _TileSize)
{
	m_TileSize = _TileSize;

	// 오브젝트 스케일 재 조정
	Vec2 vScale = m_TileSize * Vec2(m_Col, m_Row);
	Transform()->SetRelativeScale(Vec3(vScale.x, vScale.y, 1.f));
}

void CTileRender::SetSprite(UINT _Col, UINT _Row, Ptr<CSprite> _Sprite)
{
	assert(m_Col > _Col && m_Row > _Row);

	int idx = m_Col * _Row + _Col;	

	if (m_vecTileInfo[idx] == nullptr)
	{
		m_vecTileInfo[idx] = new tTileInfo;
	}

	m_vecTileInfo[idx]->Sprite = _Sprite;

	// 타일 데이터를 버퍼로 전송
	UpdateBuffer();
}

void CTileRender::SetCollision(UINT _Col, UINT _Row)
{
	assert(m_Col > _Col && m_Row > _Row);
	int idx = m_Col * _Row + _Col;
	if (m_vecTileInfo[idx]->Collision == false) {
		m_vecTileInfo[idx]->Collision = true;
	}
	else {
		m_vecTileInfo[idx]->Collision = false;
	}
}

void CTileRender::SetTileRotation(UINT _Col, UINT _Row)
{
	assert(m_Col > _Col && m_Row > _Row);
	int idx = m_Col * _Row + _Col;

	m_vecTileInfo[idx]->Rotation_z += 90.f;

	if (m_vecTileInfo[idx]->Rotation_z > 360.f) {
		m_vecTileInfo[idx]->Rotation_z = 0.f;
	}

	//타일정보 업데이트 한것 보냄
	UpdateBuffer();
}

Ptr<CSprite> CTileRender::GetSprite(UINT _Col, UINT _Row)
{

	assert(m_Col > _Col && m_Row > _Row);

	int idx = m_Col * _Row + _Col;
	if (m_vecTileInfo[idx]->Sprite == nullptr) {
		return nullptr;
	}
	else {
		return m_vecTileInfo[idx]->Sprite;
	}
}

void CTileRender::CreateTileRenderMtrl()
{
	if (nullptr == CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"TileRenderShader"))
	{
		// TileRenderShader
		Ptr<CGraphicShader> pShader = new CGraphicShader;
		pShader->CreateVertexShader(L"HLSL\\tilerender.fx", "VS_TileRender");
		pShader->CreatePixelShader(L"HLSL\\tilerender.fx", "PS_TileRender");
		pShader->SetRSType(RS_TYPE::CULL_NONE);
		pShader->SetBSType(BS_TYPE::ALPHABLEND);
		pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);
		CAssetMgr::GetInst()->AddAsset(L"TileRenderShader", pShader.Get());
	}

	if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileRenderMtrl"))
	{
		// TileRenderMtrl
		Ptr<CMaterial> pMtrl = new CMaterial;
		pMtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"TileRenderShader"));
		CAssetMgr::GetInst()->AddAsset(L"TileRenderMtrl", pMtrl.Get());
	}

	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileRenderMtrl"));
}


Ptr<CTexture> CTileRender::GetAtlasTex()
{
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		if (nullptr == m_vecTileInfo[i]->Sprite)
			continue;

		return m_vecTileInfo[i]->Sprite->GetAtlasTex();
	}

	return nullptr;
}

void CTileRender::UpdateBuffer()
{
	if (m_GpuBuffer->GetElementCount() < m_Row * m_Col)
	{
		m_GpuBuffer->Create(32, m_Row * m_Col, SB_TYPE::SRV_ONLY, true);
	}

	struct SpriteInfo {
		Vec2 vLeftTop;
		Vec2 vSlice;
		float Rotation_z;
		float padding[3];
	};
	static vector<SpriteInfo> vecInfo;
	vecInfo.clear();

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		SpriteInfo info;
		if (m_vecTileInfo[i]->Sprite.Get())
		{
			info.vLeftTop = m_vecTileInfo[i]->Sprite->GetLeftTop();
			info.vSlice = m_vecTileInfo[i]->Sprite->GetSlice();
			info.Rotation_z = m_vecTileInfo[i]->Rotation_z;
		}
		else
		{
			info.vLeftTop = Vec2(-1.f, -1.f);
			info.vSlice = Vec2(-1.f, -1.f);
			info.Rotation_z = 0.f;
		}		

		vecInfo.push_back(info);
	}

	if (!vecInfo.empty())
	{
		m_GpuBuffer->SetData(vecInfo.data(), sizeof(SpriteInfo), vecInfo.size());
	}	
}

void CTileRender::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);
		
	fwrite(&m_Col, sizeof(UINT), 1, _File);
	fwrite(&m_Row, sizeof(UINT), 1, _File);
	fwrite(&m_TileSize, sizeof(Vec2), 1, _File);
	
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		SaveAssetRef(m_vecTileInfo[i]->Sprite, _File);
	}
}

void CTileRender::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	fread(&m_Col, sizeof(UINT), 1, _File);
	fread(&m_Row, sizeof(UINT), 1, _File);
	fread(&m_TileSize, sizeof(Vec2), 1, _File);

	SetColRow(m_Col, m_Row);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i]->Sprite = LoadAssetRef<CSprite>(_File);
	}

	UpdateBuffer();
}



