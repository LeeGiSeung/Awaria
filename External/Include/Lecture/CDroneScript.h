#pragma once
#include <Engine/CScript.h>

class CDroneScript :
    public CScript
{
private:
    DRONE_STATE m_Drone_State;


public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CDroneScript);
    CDroneScript();
    ~CDroneScript();
};

