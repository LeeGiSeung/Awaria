#pragma once

#include <Engine/CScript.h>
#include "Engine/CMaterial.h"
#include <Engine/CSound.h>

class CGameObject;
class CMaterial;
class CLevel;
class CSound;

class CGeneratorScript :
    public CScript
{
private:
    GENERATOR_STATE m_Generator_state;
    GENERATOR_UI_STATE  m_ReneratorUI_state;
    CGameObject* pGenerator_Effect;
    CGameObject* FixAnima;
    CGameObject* FixVFX;

    Ptr<CSound>     pSound;

    float           ScriptRunningTime;
    float           pGenerator_Effect_CoolTime;
    float           victoryScreen_Time;
    bool            SpawnvictoryScreen;

    bool            pGenerator_Effect_spawn;

    PART            Part_Come1;
    PART            Part_Come2;
    PART            Part_Want1;
    PART            Part_Want2;

    int             dir;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    
    void GeneratorChangeLevel();
    CLevel* LoadLevel(wstring _FilePath);
    CGameObject* LoadGameObject(FILE* _File);

public:
    CLONE(CGeneratorScript);
    CGeneratorScript();
    ~CGeneratorScript();
};

