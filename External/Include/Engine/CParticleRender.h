#pragma once
#include "CRenderComponent.h"
#include "CParticleTickCS.h"

class CStructuredBuffer;

class CParticleRender :
    public CRenderComponent
{
private:
    CStructuredBuffer*      m_ParticleBuffer;
    CStructuredBuffer*      m_SpawnBuffer;
    CStructuredBuffer*      m_ModuleBuffer;

    Ptr<CParticleTickCS>    m_TickCS;
    UINT                    m_MaxParticle;      // 파티클 최대 개수

    tParticleModule         m_Module;           // 파티클 제어 설정값
    bool                    m_ModuleChanged;    // 모듈 데이터 변경점 여부
    float                   m_AccTime;          // 누적시간 체크

    Ptr<CTexture>           m_ParticleTex;      // 파티클용 이미지


public:    
    void SetParticleTex(Ptr<CTexture> _Tex) { m_ParticleTex = _Tex; }

    // Spawn
    void SetSpawnRate(float _Rate) { m_Module.SpawnRate = _Rate; m_ModuleChanged = true; }
    float GetSpawnRate() { return m_Module.SpawnRate;m_ModuleChanged = true; }
    void SetStartColor(Vec4 _Color) { m_Module.StartColor = _Color; m_ModuleChanged = true;}
    
    void SetMinScale(Vec3 _Scale) { m_Module.MinScale = _Scale;m_ModuleChanged = true; }
    void SetMaxScale(Vec3 _Scale) { m_Module.MaxScale = _Scale; m_ModuleChanged = true;}
    void SetMinLife(float _MinLife) { m_Module.MinLife = _MinLife; m_ModuleChanged = true;}
    void SetMaxLife(float _MaxLife) { m_Module.MaxLife = _MaxLife; m_ModuleChanged = true;}

    // 0 : Box, 1 : Sphere
    void SetSpawnShape(int _Shape) { m_Module.SpawnShape = _Shape; m_ModuleChanged = true;}
    void SetSpawnShapeScale(Vec3 _Scale) { m_Module.SpawnShapeScale = _Scale; m_ModuleChanged = true;}
    // 0 : Local, 1 : World;
    void SetSpaceType(int _Type) { m_Module.SpaceType = _Type; m_ModuleChanged = true;}

    void SetModlue(PARTICLE_MODULE _Module, bool _On) { m_Module.Module[(int)_Module] = _On;  m_ModuleChanged = true;}

    // AddVeolcityModule
    // 0 : Random Velocity, 1 : FromCenter, 2: ToCenter, 3 : Fixed
    void SetAddVelocityType(int _Type) {m_Module.AddVelocityType = _Type; m_ModuleChanged = true;}
    void SetMinSpeed(float _MinSpeed) { m_Module.AddMinSpeed = _MinSpeed;  m_ModuleChanged = true;}
    void SetMaxSpeed(float _MaxSpeed) { m_Module.AddMaxSpeed = _MaxSpeed;  m_ModuleChanged = true;}
    void SetFixedVelocity(Vec3 _FixedVelocityDir) { m_Module.AddVelocityFixedDir = _FixedVelocityDir; m_ModuleChanged = true;}

    // SpawnBurst
    void SetBurstParticleCount(int _Count) { m_Module.SpawnBurstCount = _Count;  m_ModuleChanged = true;}
    void SetBurstRepeatCount(int _Count) { m_Module.SpawnBurstRepeat = _Count;  m_ModuleChanged = true;}
    void SetBurstTerm(float _Term) { m_Module.SpawnBurstRepeatTime = _Term;  m_ModuleChanged = true;}
    
    // ScaleModule
    void SetStartScale(float _Start) { m_Module.StartScale = _Start;  m_ModuleChanged = true;}
    void SetEndScale(float _End) { m_Module.EndScale = _End;  m_ModuleChanged = true;}

    // DragModule
    void SetDragDestNormalizeAge(float _Age) { m_Module.DestNormalizedAge = _Age;  m_ModuleChanged = true;}
    void SetDragLimitSpeed(float _LimitSpeed) {m_Module.LimitSpeed = _LimitSpeed; m_ModuleChanged = true;}

    // NoiseForce
    void SetNoiseForceTerm(float _Term) { m_Module.NoiseForceTerm = _Term;  m_ModuleChanged = true;}
    void SetNoiseForceScale(float _Scale) { m_Module.NoiseForceScale = _Scale;  m_ModuleChanged = true;}

    // Render Module
    void SetEndColor(Vec4 _Color) { m_Module.EndColor = _Color; m_ModuleChanged = true; }
    void SetFadeOut(bool _FadeOut) { m_Module.FadeOut = _FadeOut;  m_ModuleChanged = true;}
    void SetFadOutStartRatio(float _Ratio) { m_Module.StartRatio = _Ratio;  m_ModuleChanged = true;}
    void SetVelocityAlignment(bool _Use, bool _CrossMesh) { m_Module.VelocityAlignment = _Use; m_Module.CrossMesh = _CrossMesh;  m_ModuleChanged = true;}
    
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateMtrl();
    void CalcSpawnCount();


public:
    CLONE(CParticleRender);
    CParticleRender();
    CParticleRender(const CParticleRender& _Origin);
    ~CParticleRender();
};

