#pragma once

#include <Engine/CScript.h>
class CCollider2D;
class CGameObject;

class CBigRhinoScript :
    public CScript
{
private:
    BIG_RHINO_STATE m_Rhino_State;
    Vec3            TargetPos;
    float           m_Speed;
    Vec2            MovingPos;
    Vec3            AttackPos;
    Vec3            AttackRhinoPos;
    float           Distance;

    MOVE_DIR        m_RhinoRDir;
    float           CoolTime;

    bool m_bBlockLeft;
    bool m_bBlockRight;
    bool m_bBlockUp;
    bool m_bBlockDown;

    int bBlockCount;

public:

    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CBigRhinoScript);
public:
    CBigRhinoScript();
    ~CBigRhinoScript();
};

