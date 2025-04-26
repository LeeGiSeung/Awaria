#pragma once
#include "CComponent.h"
class CLight2D :
    public CComponent
{
private:
    tLight2DInfo    m_Info;

public:
    void SetLightType(LIGHT_TYPE _Type) { m_Info.Type = _Type; }
    LIGHT_TYPE GetLightType() { return m_Info.Type; }

    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }
    float GetRadius() { return m_Info.Radius; }

    void SetAngle(float _Degree) { m_Info.Angle = XMConvertToRadians(_Degree); }
    float GetAngle() { return XMConvertToDegrees(m_Info.Angle); }

    void SetLightColor(Vec3 _Color) { m_Info.Color = _Color; }
    Vec3 GetLightColor() { return m_Info.Color; }

    void SetDir(Vec3 _vDir) { m_Info.Dir = _vDir; }
    Vec3 GetDir() { return m_Info.Dir; }

    const tLight2DInfo& GetLightInfo() { return m_Info; }

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CLight2D)
    CLight2D();
    ~CLight2D();
};

