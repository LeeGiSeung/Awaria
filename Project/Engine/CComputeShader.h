#pragma once
#include "CShader.h"
#include "CStructuredBuffer.h"

class CComputeShader :
    public CShader
{
protected:
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

    tMtrlConst                  m_Const;

    const UINT                  m_GroupPerThreadX;
    const UINT                  m_GroupPerThreadY;
    const UINT                  m_GroupPerThreadZ;

    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;


protected:
    int CreateComputeShader(const wstring& _RelativePath, const string& _FuncName);   

public:
    int Excute();

private:
    virtual int Binding() = 0;
    virtual void CalcGroupNum() = 0;
    virtual void Clear() = 0;

    CLONE_DISABLE(CComputeShader);
public:
    CComputeShader();
    CComputeShader(const wstring& _RelativePath, const string& _FuncName
                  , UINT _GroupPerX, UINT _GroupPerY, UINT _GroupPerZ);
    ~CComputeShader();
};

