#pragma once
#include <Engine/CScript.h>

class CGameObject;

class CAparatusScript :
    public CScript
{
private:
    APPARATUS_STATE     m_Apparatus_state;
    CGameObject* pPart = nullptr;

    bool SpawnPart;
    float RunningTime;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    void PlayStartFlipbook();
    void PlayIngFlipbook();
    void PlayEndFlipbook();
    void SetSpawnPart() { SpawnPart = true; }

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CAparatusScript);
public:
    CAparatusScript();
    ~CAparatusScript();
};

