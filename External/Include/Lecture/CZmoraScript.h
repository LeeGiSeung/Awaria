#pragma once
#include <Engine/CScript.h>

class CCollider2D;
class CGameObject;

class CZmoraScript :
    public CScript
{

public:
    Vec3 TargetPos;
    float m_Speed;
    float m_CoolTime;
    float m_TakeDownCoolTime;
    ZMORA_STATE m_ZmoraState;

    bool m_bBlockLeft;
    bool m_bBlockRight;
    bool m_bBlockUp;
    bool m_bBlockDown;

    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    void ResetCoolTime();

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:

    CLONE(CZmoraScript);
public:
    CZmoraScript();
    ~CZmoraScript();
};

