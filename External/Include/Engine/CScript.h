#pragma once
#include "CComponent.h"

#include "CPrefab.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
    PREFAB,
};

struct tScriptParam
{
    SCRIPT_PARAM Type;
    string       Desc;
    void*        pData;
    DWORD_PTR    Param0;
    DWORD_PTR    Param1;
    DWORD_PTR    Param2;
};

class CScript :
    public CComponent
{
private:
    const int               m_ScriptType;
    vector<tScriptParam>    m_vecScriptParam;

public:
    int GetScriptType() { return m_ScriptType; }

    void AddScriptParam(SCRIPT_PARAM _Type, const string& _Desc, void* _Data
        , DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0, DWORD_PTR _Param2 = 0)
    {
        m_vecScriptParam.push_back(tScriptParam{ _Type , _Desc , _Data , _Param0 , _Param1 , _Param2 });
    }

    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final {}
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) {}

public:
    void Instantiate(Ptr<CPrefab> _Prefab, int _LayerIdx, Vec3 _WorldPos);

public:
    virtual CScript* Clone() = 0;

public:
    CScript(int _ScriptType);
    CScript(const CScript& _Origin);
    ~CScript();
};

