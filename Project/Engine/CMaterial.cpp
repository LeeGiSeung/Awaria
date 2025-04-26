#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTexture.h"

#include "CAssetMgr.h"

CMaterial::CMaterial(bool _EngineAsset)
	: CAsset(ASSET_TYPE::MATERIAL, _EngineAsset)
	, m_Const{}
{
	
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetTexParam(TEX_PARAM _Param, const Ptr<CTexture>& _Tex)
{
	m_arrTex[_Param] = _Tex;
}

void CMaterial::Binding()
{
	m_Shader->Binding();

	for (UINT i = 0; i < TEX_PARAM::TEX_END; ++i)
	{
		if (nullptr != m_arrTex[i].Get())
		{
			m_arrTex[i]->Binding(i);
			m_Const.bTex[i] = 1;
		}
		else
		{
			ID3D11ShaderResourceView* pSRV = nullptr;
			CONTEXT->VSSetShaderResources(i, 1, &pSRV);
			CONTEXT->HSSetShaderResources(i, 1, &pSRV);
			CONTEXT->DSSetShaderResources(i, 1, &pSRV);
			CONTEXT->GSSetShaderResources(i, 1, &pSRV);
			CONTEXT->PSSetShaderResources(i, 1, &pSRV);

			m_Const.bTex[i] = 0;
		}
	}

	// 재질 상수값을 상수버퍼에 전달 및 바인딩
	static CConstBuffer* pBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pBuffer->SetData(&m_Const, sizeof(tMtrlConst));
	pBuffer->Binding();
}

int CMaterial::Save(const wstring& _FilePath)
{
	SetRelativePathByFilePath(_FilePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	// 쉐이더 정보 저장
	SaveAssetRef(m_Shader, pFile);

	// 텍스쳐 정보 저장
	for (int i = 0; i < TEX_END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);		
	}

	// 상수 데이터
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// 파일 스트림 열기 실패 시
	if (nullptr == pFile)
		return E_FAIL;

	// 쉐이더 정보
	m_Shader = LoadAssetRef<CGraphicShader>(pFile);

	// 텍스쳐 정보 저장
	for (int i = 0; i < TEX_END; ++i)
	{
		m_arrTex[i] = LoadAssetRef<CTexture>(pFile);		
	}

	// 상수 데이터
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}