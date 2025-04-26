#pragma once

#include <Engine/CScript.h>

class CMissileScript :
    public CScript
{
private:
    Vec3     m_vDir;
    float    m_Speed;
    
public:
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CMissileScript);
    CMissileScript();
    ~CMissileScript();
};

