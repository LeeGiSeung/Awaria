#ifndef _PARTICLE_FUNC
#define _PARTICLE_FUNC

#include "value.fx"



#define PI 3.1415926535f

float3 GetRandom(float _NomalizedThreadID, in Texture2D _NoiseTex)
{
    float2 vUV = (float2) 0.f;
        
    vUV.x = _NomalizedThreadID + g_EngineTime * 0.1f;
    vUV.y = 0.5f + sin(vUV.x * 100.f * PI) * 0.5f;
        
    float3 vRandom = _NoiseTex.SampleLevel(g_sam_0, vUV, 0);    
    return vRandom;
}


void ParticleInit(inout tParticle _Particle, in tParticleModule _Module
                , in Texture2D _NoiseTex, int _IsNoiseBind, float _NomalizedThreadID)
{   
    _Particle = (tParticle) 0.f;
    
    // ��ƼŬ Ȱ��ȭ
    _Particle.Active = true;
    
    // Spawm Module
    // Box
    if (0 == _Module.SpawnShape)
    {
        float3 vRandom = GetRandom(_NomalizedThreadID, _NoiseTex);
        
        // ���� ������ -0.5 ~ 0.5f �� ���� ��, �������� ũ�⸦ ���ؼ� ��ġ���� ���Ѵ�.
        float3 vSpanwPos = (vRandom - 0.5f) * _Module.SpawnShapeScale;
        _Particle.LocalPos.xyz = vSpanwPos;
    }
    // Sphere
    else if (1 == _Module.SpawnShape)
    {
        
    }
  
    // ��ƼŬ Life ���� ����
    _Particle.Age = 0.f;
    float3 vRandom = GetRandom(_NomalizedThreadID + 0.1f, _NoiseTex);
    _Particle.Life = (_Module.MaxLife - _Module.MinLife) * vRandom.r + _Module.MinLife;
        
    // ��ƼŬ �ʱ� �� ����
    _Particle.Color = _Module.StartColor;
    
    // ��ƼŬ �ʱ� ũ�� ����
    _Particle.WorldInitScale.xyz = _Particle.WorldScale.xyz = (_Module.MaxScale - _Module.MinScale) * vRandom.g + _Module.MinScale;
        
    // ��ƼŬ ���� ����
    _Particle.Mass = 1.f;
    
    // ��ƼŬ �ʱ� �ӵ� �߰�
    _Particle.Velocity = (float4) 0.f;
                
    if (_Module.Module[2])
    {
        float3 vRandom = GetRandom(_NomalizedThreadID + 0.2f, _NoiseTex);
        
        float fSpeed = _Module.AddMinSpeed + (_Module.AddMaxSpeed - _Module.AddMinSpeed) * vRandom.x;
                        
        // Random
        if (0 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(vRandom - 0.5f) * fSpeed;
        // FromCenter
        else if (1 == _Module.AddVelocityType)                        
            _Particle.Velocity.xyz = normalize(_Particle.LocalPos) * fSpeed;
        // ToCenter
        else if (2 == _Module.AddVelocityType)
            _Particle.Velocity.xyz = -normalize(_Particle.LocalPos) * fSpeed;
        // Fixed
        else
            _Particle.Velocity.xyz = normalize(_Module.AddVelocityFixedDir) * fSpeed;
    }
    
    // Particle �� World ��ǥ ���
    _Particle.WorldPos.xyz = _Particle.LocalPos.xyz + _Module.ObjectWorldPos;
}

#endif