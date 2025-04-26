#pragma once
#include "CAsset.h"


class CShader :
    public CAsset
{
protected:
    ComPtr<ID3DBlob>        m_ErrBlob;


public:
    virtual int Save(const wstring& _FilePath) { return S_OK; }
    virtual int Load(const wstring& _FilePath) { return S_OK; }
    CLONE_DISABLE(CShader);
public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
};

