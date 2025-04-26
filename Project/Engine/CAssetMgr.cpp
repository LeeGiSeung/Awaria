#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"

CAssetMgr::CAssetMgr()
	: m_AssetChanged(false)
{

}

CAssetMgr::~CAssetMgr()
{

}

void CAssetMgr::AddAsset(const wstring& _Key, Ptr<CAsset> _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();

	map<wstring, Ptr<CAsset>>::iterator iter  = m_mapAsset[Type].find(_Key);

	if (iter == m_mapAsset[Type].end()) {
		int a = 0;
	}

	assert(iter == m_mapAsset[Type].end());

	_Asset->m_Key = _Key;
	m_mapAsset[Type].insert(make_pair(_Key, _Asset));

	m_AssetChanged = true;
}

Ptr<CAsset> CAssetMgr::FindAsset(ASSET_TYPE _Type, const wstring& _Key)
{	
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[_Type].find(_Key);

	if (iter == m_mapAsset[_Type].end())
		return nullptr;

	return iter->second.Get();
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height
									, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);
	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Width, _Height, _Format, _BindFlag, _Usage)))
	{
		MessageBox(nullptr, _Key.c_str(), L"�ؽ��� ���� ����", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);
	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, _Key.c_str(), L"�ؽ��� ���� ����", MB_OK);
		return nullptr;
	}

	AddAsset(_Key, pTex.Get());

	return pTex;	
}

void CAssetMgr::GetAssetName(ASSET_TYPE _Type, vector<string>& _vecNames)
{
	map<wstring, Ptr<CAsset>>& mapAsset = m_mapAsset[_Type];

	for (const auto& pair : mapAsset)
	{
		_vecNames.push_back(string(pair.first.begin(), pair.first.end()));
	}
}

void CAssetMgr::DeleteAsset(Ptr<CAsset> _Asset)
{
	DeleteAsset(_Asset->GetType(), _Asset->GetKey());
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);

	if (iter == m_mapAsset[(UINT)_Type].end())
	{
		assert(nullptr);
		return;
	}

	m_mapAsset[(UINT)_Type].erase(iter);

	m_AssetChanged = true;
}

Ptr<CSprite> CAssetMgr::LoadSprite(const wstring& _Key, const wstring& _RelativePath)
{
	//CAsset* pSprite = FindAsset(ASSET_TYPE::SPRITE, _Key);
	Ptr<CSprite> pSprite = FindAsset<CSprite>(_Key);

	// �̹� �ε��� ���� �ִ� Texture ���
	if (nullptr != pSprite)
	{
		// �ε��� �ؽ��ĸ� ��ȯ
		//return (CSprite*)pSprite;
		return pSprite;
	}
	wstring ContentPath = CPathMgr::GetContentPath();

	pSprite = new CSprite;
	pSprite->Load(ContentPath + _RelativePath);
	//pSprite->Load(ContentPath + _Key);

	// ���¿�, �ڽ��� ���¸Ŵ����� ��ϵɶ� ���� Ű���� �ε��� �� ����� ��θ� �������ش�.
	pSprite->SetKey(_Key);
	pSprite->SetRelativePath(_RelativePath);

	// �����̳ʿ� �ؽ��� ���
	//m_mapAsset[SPRITE].insert(make_pair(_Key, pSprite));
	AddAsset(_Key, pSprite.Get());

	return pSprite;
}

Ptr<CFlipbook> CAssetMgr::LoadFlipbook(const wstring& _Key, const wstring& _RelativePath)
{
	Ptr<CFlipbook> pFlipbook = FindAsset<CFlipbook>(_Key);

	// �̹� �ε��� ���� �ִ� Texture ���
	if (nullptr != pFlipbook)
	{
		// �ε��� �ؽ��ĸ� ��ȯ
		return pFlipbook;
	}

	wstring ContentPath = CPathMgr::GetContentPath();

	pFlipbook = new CFlipbook;
	pFlipbook->Load(ContentPath + _RelativePath);

	// ���¿�, �ڽ��� ���¸Ŵ����� ��ϵɶ� ���� Ű���� �ε��� �� ����� ��θ� �������ش�.
	pFlipbook->SetKey(_Key);
	pFlipbook->SetRelativePath(_RelativePath);

	// �����̳ʿ� �ؽ��� ���
	AddAsset(_Key, pFlipbook.Get());

	return pFlipbook;
}
