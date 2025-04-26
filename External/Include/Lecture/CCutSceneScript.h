#pragma once
#include <Engine/CScript.h>

class CCutSceneScript :
    public CScript
{
private:
    float           m_Speed;
    float           m_PaperBurn;
    Ptr<CPrefab>    m_Prefab;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CCutSceneScript);
public:
    CCutSceneScript();
    ~CCutSceneScript();
};

