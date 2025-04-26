#pragma once
#include <Engine/CScript.h>

class CSmallRhinoScript :
    public CScript
{

private:
    SMALL_RHINO_STATE m_Rhino_State;
    Vec3            TargetPos;
    float           m_Speed;
    Vec2            MovingPos;
    Vec3            AttackPos;
    Vec3            AttackRhinoPos;
    float           Distance;
    float           CoolTime;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CSmallRhinoScript);
public:
    CSmallRhinoScript();
    ~CSmallRhinoScript();
};

