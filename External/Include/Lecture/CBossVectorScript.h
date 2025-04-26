#pragma once
#include <Engine/CScript.h>
class CBossVectorScript :
    public CScript
{
private:
    float CoolTime;
    float WaitTime;
    int SpawnCount;
    int SpawnMonsterType;


    int SkulLeftRight;
    BOSS_VECTOR_STATE m_Vector_state;


public:

    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CBossVectorScript);
public:
    CBossVectorScript();
    ~CBossVectorScript();
};

