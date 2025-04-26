#include "pch.h"
#include "CDevice.h"

#include "CConstBuffer.h"

#include "CAssetMgr.h"
#include "CTexture.h"


CDevice::CDevice()
    : m_hMainWnd(nullptr)
    , m_ConstBuffer{}
    , m_RSState{}
{

}

CDevice::~CDevice()
{
    for (int i = 0; i < (UINT)CB_TYPE::END; ++i)
    {
        if (nullptr != m_ConstBuffer[i])
            delete m_ConstBuffer[i];
    }
}

int CDevice::Init(HWND _OutputWnd, Vec2 _vResolution)
{
    m_hMainWnd = _OutputWnd;
    m_RenderResolution = _vResolution;

    g_global.g_Resolution = m_RenderResolution;

    UINT Flag = 0;
#ifdef _DEBUG
    Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
        , Flag, 0, 0, D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), &level, m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 오류", MB_OK);
        return E_FAIL;
    }

    // 스왚체인 생성
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 오류", MB_OK);
        return E_FAIL;
    }

    // View 생성
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 오류", MB_OK);
        return E_FAIL;
    }

    // RaterizerState 종류별로 생성
    CreateRasterizerState();

    // DepthStencilState 생성
    CreateDepthStencilState();

    // BlendState 생성
    CreateBlendState();

    // 상수버퍼를 각 상수 레지스터에 대응할 수 있도록 하나씩 만들어 둠
    CreateConstBuffer();

    // ViewPort 설정
    D3D11_VIEWPORT ViewPort = { };

    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;

    ViewPort.Width = m_RenderResolution.x;
    ViewPort.Height = m_RenderResolution.y;

    // 깊이 텍스쳐에 저장하는 깊이값의 범위
    ViewPort.MinDepth = 0.f;
    ViewPort.MaxDepth = 1.f;

    // 뷰포트 설정값 전달
    m_Context->RSSetViewports(1, &ViewPort);

    // 출력 렌더타겟 및 출력 깊이타겟 설정
    m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

    // 샘플러 생성 및 바인딩
    CreateSamplerState();

    return S_OK;
}

void CDevice::ClearTarget()
{
    float Color[4] = {0.f, 0.f, 0.f, 1.f };
    m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), Color);
    m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
}

int CDevice::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC Desc = {};
        
    Desc.OutputWindow = m_hMainWnd;   // 생성된 스왚체인이 이미지를 출력시킬 목적지 윈도우
    Desc.Windowed = true;             // 윈도우 모드, 전체화면 모드
    
    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;     

    Desc.BufferCount = 1;             // 출력 Buffer 개수         
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Buffer 용도
    Desc.BufferDesc.Width = (UINT)m_RenderResolution.x;
    Desc.BufferDesc.Height = (UINT)m_RenderResolution.y;
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    Desc.BufferDesc.RefreshRate.Numerator = 60;
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    
    ComPtr<IDXGIDevice> pDevice = nullptr;
    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    ComPtr<IDXGIFactory> pFactory = nullptr;

    m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);
    pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

    if (FAILED(pFactory->CreateSwapChain(pDevice.Get(), &Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateView()
{
    // RenderTargetView 생성
    // 이미 생성되어있는 렌더타겟 텍스쳐를 스왚체인으로 부터 얻어온다.
    ComPtr<ID3D11Texture2D> pRTTex = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pRTTex.GetAddressOf());
        
    // SwapChain 이 보유하고있는 ID3D11Texture2D 객체를 CTexture 클래스로 전환한다.
    m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", pRTTex);
        
    // DepthStencil 리소스(텍스쳐) 생성    
    m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
                                     , (UINT)m_RenderResolution.x, (UINT)m_RenderResolution.y
                                     , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);  

    return S_OK;
}

void CDevice::CreateConstBuffer()
{
    m_ConstBuffer[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(CB_TYPE::TRANSFORM, sizeof(tTransform));

    m_ConstBuffer[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::MATERIAL]->Create(CB_TYPE::MATERIAL, sizeof(tMtrlConst));

    m_ConstBuffer[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer;
    m_ConstBuffer[(UINT)CB_TYPE::GLOBAL]->Create(CB_TYPE::GLOBAL, sizeof(tGlobal));    
}

void CDevice::CreateRasterizerState()
{
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    D3D11_RASTERIZER_DESC Desc = {};
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());
       
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());    
}

void CDevice::CreateDepthStencilState()
{
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    D3D11_DEPTH_STENCIL_DESC Desc = {};

    // Less Equal
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 기록 O
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

    // Greater
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 깊이 기록 O
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

    // NoWrite
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS; 
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 기록 X
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

    // NoTest NoWrite 
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;          // 깊이 항상 통과(검사 X)
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // 깊이 기록 X
    Desc.StencilEnable = false;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WIRITE].GetAddressOf());
}

void CDevice::CreateBlendState()
{
    // Src : 1, Dst : 0
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

    // AlphaBlend
    D3D11_BLEND_DESC Desc = {};

    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = true;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // RGB 혼합 계수 설정
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;          // Src 와 Dst 를 더하기    
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // Src 계수는 SrcAlpha
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // Dst 계수는 (1 - SrcAlpha)

    // A 혼합 계수 설정
    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    //블랜드 설정 blend set
    //Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    //Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;    
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());
}

void CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC; // 이방성 필터링
    // MipMap
    Desc.MinLOD = 0; 
    Desc.MaxLOD = 0;    
    DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf());


    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT; // 2D 필터
    // MipMap
    Desc.MinLOD = 0;
    Desc.MaxLOD = 0;
    DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
}
