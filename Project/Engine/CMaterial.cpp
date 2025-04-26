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

	// ���� ������� ������ۿ� ���� �� ���ε�
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

	// ���̴� ���� ����
	SaveAssetRef(m_Shader, pFile);

	// �ؽ��� ���� ����
	for (int i = 0; i < TEX_END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);		
	}

	// ��� ������
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	// ���� ��Ʈ�� ���� ���� ��
	if (nullptr == pFile)
		return E_FAIL;

	// ���̴� ����
	m_Shader = LoadAssetRef<CGraphicShader>(pFile);

	// �ؽ��� ���� ����
	for (int i = 0; i < TEX_END; ++i)
	{
		m_arrTex[i] = LoadAssetRef<CTexture>(pFile);		
	}

	// ��� ������
	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	fclose(pFile);

	return S_OK;
}