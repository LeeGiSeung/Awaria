#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh(bool _EngineAsset)
	: CAsset(MESH, _EngineAsset)
	, m_VBDesc{}
	, m_IBDesc{}
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (nullptr != m_VtxSysMem)
		delete m_VtxSysMem;

	if (nullptr != m_IdxSysMem)
		delete m_IdxSysMem;
}

int CMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;


	// 버퍼 크기 설정
	m_VBDesc.ByteWidth = sizeof(Vtx) * _VtxCount;

	// 생성 이후엔 데이터 이동 없음
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = _VtxSysMem;

	DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf());


	// 버퍼 크기 설정
	m_IBDesc.ByteWidth = sizeof(UINT) * _IdxCount;

	// 생성 이후엔 데이터 이동 없음
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Sub.pSysMem = _IdxSysMem;

	DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf());


	// System 메모리에 정점과 인덱스 정보 기록
	m_VtxSysMem = new Vtx[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	memcpy(m_VtxSysMem, _VtxSysMem, sizeof(Vtx) * m_VtxCount);
	memcpy(m_IdxSysMem, _IdxSysMem, sizeof(UINT) * m_IdxCount);

	return S_OK;
}


void CMesh::Binding()
{
	UINT Stride = sizeof(Vtx);
	UINT Offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(UINT _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}

