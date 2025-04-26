#pragma once
#include "CAsset.h"

class CTexture :
    public CAsset
{
private:
    ScratchImage                        m_Image; // DirectxTex 라이브러리에서 제공, 이미지파일을 SystemMem 로 로딩하는 기능이 구현되어있음
    ComPtr<ID3D11Texture2D>             m_Tex2D; // GPU 메모리에 저장된 텍스쳐를 관리하는 객체
    D3D11_TEXTURE2D_DESC                m_Desc;  // Texture2D 정보

    ComPtr<ID3D11RenderTargetView>	    m_RTV;   // 렌더타겟으로 전달하는 용도
    ComPtr<ID3D11DepthStencilView>	    m_DSV;   // 뎊스 스텐실 용도로 전달하는 용도
    ComPtr<ID3D11ShaderResourceView>    m_SRV;   // GraphicShader 에서 리소스로 사용하는 용도(t)
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;   // ComputeShader 에서 읽기 쓰기가 가능한 용도(u)

    int                                 m_RecentSRV_CS;
    int                                 m_RecentUAV_CS;

public:
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

    ComPtr<ID3D11Texture2D>          GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11RenderTargetView>	 GetRTV()   {return  m_RTV;}
    ComPtr<ID3D11DepthStencilView>	 GetDSV()   {return  m_DSV;}
    ComPtr<ID3D11ShaderResourceView> GetSRV()   {return  m_SRV;}
    ComPtr<ID3D11UnorderedAccessView>GetUAV()   { return  m_UAV;}

public:
    // GraphicShader, t 레지스터
    void Binding(UINT _RegisterNum);

    // ComputeShader, t 레지스터
    void Binding_CS_SRV(UINT _RegisterNum);

    // ComputeShader, u 레지스터
    void Binding_CS_UAV(UINT _RegisterNum);

    void Clear_CS_SRV(int _RegisterNum = -1);
    void Clear_CS_UAV();

   
private:
    // _Flag : D3D11_BIND_FLAG
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _format, UINT _Flag, D3D11_USAGE _usage = D3D11_USAGE_DEFAULT);    
    int Create(ComPtr<ID3D11Texture2D> _Tex2D);

    virtual int Load(const wstring& _FilePath) override;
public:
    virtual int Save(const wstring& _FilePath) override;
    
public:
    CLONE_DISABLE(CTexture);
public:
    CTexture(bool _EngineAsset = false);
    ~CTexture();

    friend class CAssetMgr;
};

