#pragma once
#include "CEntity.h"

class CAsset :
    public CEntity
{
private:
    const ASSET_TYPE    m_Type;
    int                 m_RefCount; // 참조 횟수(0 이되면 스스로 삭제)

    wstring             m_Key;
    wstring             m_RelativePath;

    const bool          m_EngineAsset;

public:
    ASSET_TYPE GetType() const { return m_Type; }

    bool IsEngineAsset() { return m_EngineAsset; }

    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }

private:
    void AddRef() { ++m_RefCount; }
    void Release()
    {
        --m_RefCount;
        if (m_RefCount <= 0)
        {
            delete this;
        }         
    }

protected:
    void CheckExt(const wstring& _Ext, wstring& _strFilePath);
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _Path) { m_RelativePath = _Path; }
    void SetRelativePathByFilePath(const wstring& _FullPath);

public:
    virtual int Save(const wstring& _FilePath) = 0;

private:
    virtual int Load(const wstring& _FilePath) = 0;
    virtual CAsset* Clone() = 0;
public:
    CAsset(ASSET_TYPE _Type, bool _EngineAsset = false);
    ~CAsset();

    template<typename T>
    friend class Ptr;

    friend class CAssetMgr;
};

