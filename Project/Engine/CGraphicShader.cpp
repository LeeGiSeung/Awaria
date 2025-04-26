#include "pch.h"
#include "CGraphicShader.h"

#include "CPathMgr.h"
#include "CDevice.h"



CGraphicShader::CGraphicShader()
	: CShader(ASSET_TYPE::GRAPHIC_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_Domain(SHADER_DOMAIN::DOMAIN_QPAQUE)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
{
}

CGraphicShader::~CGraphicShader()
{
}

int CGraphicShader::CreateVertexShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetContentPath() + _RelativePath;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
							    , nullptr
							    , D3D_COMPILE_STANDARD_FILE_INCLUDE
							    , _FuncName.c_str(), "vs_5_0", Flag, 0
							    , m_VSBlob.GetAddressOf()
							    , m_ErrBlob.GetAddressOf())))
	{
		// 컴파일 실패하면 메세지 박스에 컴파일 실패내용 띄우기
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "쉐이더 컴파일 실패", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "쉐이더 파일 경로가 옮바르지 않습니다.", "쉐이더 컴파일 실패", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
										, m_VSBlob->GetBufferSize()
										, nullptr
										, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}


	// Input Layout 도 같이 생성	
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[3] = {};

	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;	// 크기		
	LayoutDesc[0].InputSlot = 0;						// 버퍼의 인덱스
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;
	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;				// 시작 위치

	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// 크기		
	LayoutDesc[1].InputSlot = 0;							// 버퍼의 인덱스
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;
	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12;					// 시작 위치

	LayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;	// 크기		
	LayoutDesc[2].InputSlot = 0;						// 버퍼의 인덱스
	LayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate = 0;
	LayoutDesc[2].SemanticName = "TEXCOORD";
	LayoutDesc[2].SemanticIndex = 0;
	LayoutDesc[2].AlignedByteOffset = 28;			// 시작 위치


	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 3
										, m_VSBlob->GetBufferPointer()
										, m_VSBlob->GetBufferSize(), m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicShader::CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetContentPath() + _RelativePath;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
								, nullptr
								, D3D_COMPILE_STANDARD_FILE_INCLUDE
								, _FuncName.c_str(), "gs_5_0", Flag, 0
								, m_GSBlob.GetAddressOf()
								, m_ErrBlob.GetAddressOf())))
	{
		// 컴파일 실패하면 메세지 박스에 컴파일 실패내용 띄우기
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "쉐이더 컴파일 실패", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "쉐이더 파일 경로가 옮바르지 않습니다.", "쉐이더 컴파일 실패", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateGeometryShader( m_GSBlob->GetBufferPointer()
										   , m_GSBlob->GetBufferSize()
										   , nullptr
										   , m_GS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CGraphicShader::CreatePixelShader(const wstring& _RelativePath, const string& _FuncName)
{	
	wstring strFilePath = CPathMgr::GetContentPath() + _RelativePath;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
								, nullptr
								, D3D_COMPILE_STANDARD_FILE_INCLUDE
								, _FuncName.c_str(), "ps_5_0", Flag, 0
								, m_PSBlob.GetAddressOf()
								, m_ErrBlob.GetAddressOf())))
	{
		// 컴파일 실패하면 메세지 박스에 컴파일 실패내용 띄우기
		if (nullptr != m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "쉐이더 컴파일 실패", MB_OK);
		}
		else
		{
			MessageBoxA(nullptr, "쉐이더 파일 경로가 옮바르지 않습니다.", "쉐이더 컴파일 실패", MB_OK);
		}
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader( m_PSBlob->GetBufferPointer()
										, m_PSBlob->GetBufferSize()
										, nullptr
										, m_PS.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicShader::Binding()
{
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);		

	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType).Get());
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
	//이거 수정
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);
}

void CGraphicShader::AddScalarParam(SCALAR_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0, DWORD_PTR _Param1)
{
	tScalarParam param = {};

	param.Type = _Type;
	param.Desc = _Desc;
	param.Param0 = _Param0;
	param.Param1 = _Param1;

	m_ScalarParam.push_back(param);
}

void CGraphicShader::AddTexParam(TEX_PARAM _Type, const wstring& _Desc, DWORD_PTR _Param0, DWORD_PTR _Param1)
{
	tTexParam param = {};

	param.Type = _Type;
	param.Desc = _Desc;
	param.Param0 = _Param0;
	param.Param1 = _Param1;

	m_TexParam.push_back(param);
}