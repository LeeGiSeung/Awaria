#pragma once
#include <Engine/CScript.h>

class CCollider2D;
class CGameObject;

class CZmoraScript :
    public CScript
{
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:

    CLONE(CZmoraScript);
public:
    CZmoraScript();
    ~CZmoraScript();
};

