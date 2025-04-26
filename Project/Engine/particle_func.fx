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
    
    // 파티클 활성화
    _Particle.Active = true;
    
    // Spawm Module
    // Box
    if (0 == _Module.SpawnShape)
    {
        float3 vRandom = GetRandom(_NomalizedThreadID, _NoiseTex);
        
        // 랜덤 범위를 -0.5 ~ 0.5f 로 변경 후, 스폰영역 크기를 곱해서 위치값을 구한다.
        float3 vSpanwPos = (vRandom - 0.5f) * _Module.SpawnShapeScale;
        _Particle.LocalPos.xyz = vSpanwPos;
    }
    // Sphere
    else if (1 == _Module.SpawnShape)
    {
        
    }
  
    // 파티클 Life 랜덤 설정
    _Particle.Age = 0.f;
    float3 vRandom = GetRandom(_NomalizedThreadID + 0.1f, _NoiseTex);
    _Particle.Life = (_Module.MaxLife - _Module.MinLife) * vRandom.r + _Module.MinLife;
        
    // 파티클 초기 색 설정
    _Particle.Color = _Module.StartColor;
    
    // 파티클 초기 크기 설정
    _Particle.WorldInitScale.xyz = _Particle.WorldScale.xyz = (_Module.MaxScale - _Module.MinScale) * vRandom.g + _Module.MinScale;
        
    // 파티클 질량 설정
    _Particle.Mass = 1.f;
    
    // 파티클 초기 속도 추가
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
    
    // Particle 의 World 좌표 계산
    _Particle.WorldPos.xyz = _Particle.LocalPos.xyz + _Module.ObjectWorldPos;
}

#endif