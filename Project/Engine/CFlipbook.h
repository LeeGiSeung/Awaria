#pragma once
#include "CAsset.h"

#include "CSprite.h"

class CFlipbook :
    public CAsset
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;

public:
    void AddSprite(Ptr<CSprite> _Sprite) { m_vecSprite.push_back(_Sprite); }
    int GetSpriteCount() { return (int)m_vecSprite.size(); }
    Ptr<CSprite> GetSprite(int _Idx) { return m_vecSprite[_Idx]; }

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

public:
    CLONE_DISABLE(CFlipbook);
public:
    CFlipbook(bool _EngineAsset = false);
    ~CFlipbook();
};

