#pragma once

#include <Engine/CScript.h>
#include "Engine/CMaterial.h"

class CGameObject;
class CMaterial;

class CGeneratorScript :
    public CScript
{
private:
    GENERATOR_STATE m_Generator_state;
    GENERATOR_UI_STATE  m_ReneratorUI_state;

    float           ScriptRunningTime;

    PART            Part_Come1;
    PART            Part_Come2;
    PART            Part_Want1;
    PART            Part_Want2;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

public:
    CLONE(CGeneratorScript);
    CGeneratorScript();
    ~CGeneratorScript();
};

