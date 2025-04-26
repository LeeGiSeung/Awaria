#pragma once

#include "CAsset.h"
#include "assets.h"
#include "CPathMgr.h"

class CAssetMgr
	: public singleton<CAssetMgr>
{
	SINGLE(CAssetMgr)
private:
	map<wstring, Ptr<CAsset>>	m_mapAsset[ASSET_END];

public:
	void Init();
	void CreateEngineMesh();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void CreateEngineMaterial();
	void CreateEngineSprite();

	bool						m_AssetChanged;

public:
	void AddAsset(const wstring& _Key, Ptr<CAsset> _Asset);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	Ptr<CAsset> FindAsset(ASSET_TYPE _Type, const wstring& _Key);

	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath = L"");
	
	// _BindFlag : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
	Ptr<CTexture> CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _tex2D);

	const map<wstring, Ptr<CAsset>>& GetAssets(ASSET_TYPE _AssetType) { return m_mapAsset[(UINT)_AssetType]; }

	void GetAssetName(ASSET_TYPE _Type, vector<string>& _vecNames);

	void DeleteAsset(Ptr<CAsset> _Asset);
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

	Ptr<CSprite> LoadSprite(const wstring& _Key, const wstring& _RelativePath);
	//플립북 저장 및 불러오기
	Ptr<CFlipbook> LoadFlipbook(const wstring& _Key, const wstring& _RelativePath);


	bool IsAssetChanged()
	{
		bool Changed = m_AssetChanged;
		m_AssetChanged = false;
		return Changed;
	}

};

template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (is_same_v<CMesh, T>)
		return MESH;
	else if constexpr (is_same_v<CTexture, T>)
		return TEXTURE;
	else if constexpr (is_same_v<CMaterial, T>)
		return MATERIAL;
	else if constexpr (is_same_v<CGraphicShader, T>)
		return GRAPHIC_SHADER;
	else if constexpr (is_same_v<CComputeShader, T>)
		return COMPUTE_SHADER;
	else if constexpr (is_same_v<CSprite, T>)
		return SPRITE;
	else if constexpr (is_same_v<CFlipbook, T>)
		return FLIPBOOK;
	else if constexpr (is_same_v<CPrefab, T>)
		return PREFAB;
	else if constexpr (is_same_v<CSound, T>)
		return SOUND;
}

template<typename T>
inline Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[Type].find(_Key);

	if (iter == m_mapAsset[Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
inline Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	wstring RelativePath = _RelativePath;
	if (_RelativePath.empty())
	{
		RelativePath = _Key;
	}

	Ptr<T> pAsset = FindAsset<T>(_Key);
		
	if constexpr (!std::is_same_v<T, CComputeShader>)
	{
		if (nullptr != pAsset)
			return pAsset;

		wstring ContentPath = CPathMgr::GetContentPath();

		pAsset = new T;
		if (FAILED(pAsset->Load(ContentPath + RelativePath)))
		{
			MessageBox(nullptr, L"에셋 로딩 실패", L"에셋 로딩 에러", MB_OK);
			return nullptr;
		}

		pAsset->m_RelativePath = RelativePath;
		AddAsset(_Key, pAsset.Get());		
	}	

	return pAsset;
}


template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	// 에셋을 가리키고 있었는지 아닌지 저장
	int bAsset = !!_Asset.Get();
	fwrite(&bAsset, sizeof(int), 1, _File);

	if (!bAsset)
	{
		return;
	}

	wstring Key = _Asset->GetKey();
	wstring Path = _Asset->GetRelativePath();

	SaveWString(Key, _File);
	SaveWString(Path, _File);
}


template<typename T>
Ptr<T> LoadAssetRef(FILE* _File)
{
	// 이전에 에셋을 가리키고 있었는지 확인
	int bAsset = 0;
	fread(&bAsset, sizeof(int), 1, _File);
	if (!bAsset)
		return nullptr;

	wstring Key, Path;
	LoadWString(Key, _File);
	LoadWString(Path, _File);

	return CAssetMgr::GetInst()->Load<T>(Key, Path);
}