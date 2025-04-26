#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    float   m_CamSpeed;

public:
    virtual void Tick() override;

private:
    void Move_Perspective();
    void Move_OrthoGraphic();


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;



public:
    CLONE(CCameraMoveScript);
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

