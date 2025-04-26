#pragma once
#include <Engine/CScript.h>

class CSkulScript :
    public CScript
{
    SKUL_STATE      m_Skul_State;
    float           m_Speed;
    float           Distance;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CSkulScript);
public:
    CSkulScript();
    ~CSkulScript();
};

