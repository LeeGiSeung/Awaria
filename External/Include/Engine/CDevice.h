#pragma once

#include "CTexture.h"

class CConstBuffer;
class CDevice
	: public singleton<CDevice>
{
	SINGLE(CDevice);
private:
	HWND							m_hMainWnd;
	Vec2							m_RenderResolution;		// ����Ÿ�� �ػ�
	
	ComPtr<ID3D11Device>			m_Device;				// GPU �޸� ����, DirectX ��ü ����
	ComPtr<ID3D11DeviceContext>		m_Context;				// GPU ��� ����

	ComPtr<IDXGISwapChain>			m_SwapChain;			// Window ȭ�鿡 ������

	Ptr<CTexture>					m_RTTex;
	Ptr<CTexture>					m_DSTex;

	CConstBuffer*					m_ConstBuffer[(UINT)CB_TYPE::END];

	ComPtr<ID3D11RasterizerState>   m_RSState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BS_TYPE::END];

	ComPtr<ID3D11SamplerState>		m_Sampler[2];			// ���ø�(���� ����)
	
	

public:
	int Init(HWND _OutputWnd, Vec2 _vResolution);
	void ClearTarget();
	void Present();

	ComPtr<ID3D11Device> GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; } 
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_ConstBuffer[(UINT)_Type]; }
	Vec2 GetRenderResolution() { return m_RenderResolution; }
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState>	GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

private:
	int CreateSwapChain();
	int CreateView();
	void CreateConstBuffer();
	void CreateRasterizerState();
	void CreateDepthStencilState();
	void CreateBlendState();
	void CreateSamplerState();
};

