#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "particle_func.fx"

RWStructuredBuffer<tParticle> g_Buffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_Spawn : register(u1);
StructuredBuffer<tParticleModule> g_Module : register(t20);

#define MAX_PARTICLE g_int_0
#define STOP         g_int_1



[numthreads(256, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_PARTICLE <= _ID.x)
        return;
    
    float EngineDT = g_EngineDT;
    if (STOP)
    {
        EngineDT = 0.f;
    }    
    
    // 스레드가 담당한 파티클이 비활성화 상태라면
    if (false == g_Buffer[_ID.x].Active)
    {
        int Success = 0;
        while (g_Spawn[0].SpawnCount)
        {
            int SpawnCount = g_Spawn[0].SpawnCount;            
            int Input = SpawnCount - 1;
            int Origin = 0;
        
            // Atomic 함수, 스레드가 하나씩 순차적으로 실행
            InterlockedCompareExchange(g_Spawn[0].SpawnCount, SpawnCount, Input, Origin);
            //InterlockedExchange(g_Spawn[0].SpawnCount, Input, Origin);
        
            // 최초 진입한 스레드만 Origin 값을 1을 가져온다.
            if (Origin == SpawnCount)
            {
                Success = 1;                
                break;
            }
        }
        
        // 활성화된 파티클 초기화 작업
        if (Success)
        {                        
            float NormalizedThreadID = (float) _ID.x / (float) MAX_PARTICLE;
            ParticleInit(g_Buffer[_ID.x], g_Module[0], g_tex_0, g_btex_0, NormalizedThreadID);
        }
    }    
    
    else
    {        
        tParticle Particle = g_Buffer[_ID.x];
        
        // Normalized Age : 전체 수명 대비 현재 나이 정규화 비율값
        Particle.NormalizedAge = Particle.Age / Particle.Life;
                
        // 이번 프레임에 누적된 힘을 계산하기 전에, 힘을 초기화 해준다.
        Particle.Force.xyz = float3(0.f, 0.f, 0.f);
        
        // Noise Force Module
        if (g_Module[0].Module[5])
        {
            // 일정 시간마다 Noise Force 의 방향을 랜덤하게 구함
            if (g_Module[0].NoiseForceTerm <= Particle.NoiseForceAccTime)
            {
                Particle.NoiseForceAccTime -= g_Module[0].NoiseForceTerm;
                
                float NormalizedThreadID = (float) _ID.x / (float) MAX_PARTICLE;
                
                float3 vRandom = GetRandom(NormalizedThreadID, g_tex_0);
                float3 vNoiseForce = normalize(vRandom.xyz - 0.5f);

                Particle.NoiseForceDir = vNoiseForce;
            }
                        
            Particle.Force.xyz += Particle.NoiseForceDir * g_Module[0].NoiseForceScale;
            Particle.NoiseForceAccTime += EngineDT;
        }
        
        // 속도에 따른 이동
        // Particle 에 주어진 힘에 따른 가속도 계산
        float3 vAccel = Particle.Force / Particle.Mass;
        
        // 가속도에 따른 속도의 변화
        Particle.Velocity.xyz += vAccel * EngineDT;
                
        // Velocity 에 따른 이동 구현
        if (0 == g_Module[0].SpaceType)
        {
            // Space 가 Local 이라면
            Particle.LocalPos.xyz += Particle.Velocity * EngineDT;
            Particle.WorldPos.xyz = Particle.LocalPos.xyz + g_Module[0].ObjectWorldPos.xyz;
        }
        else
        {
            Particle.LocalPos += Particle.Velocity * EngineDT;
            Particle.WorldPos += Particle.Velocity * EngineDT;
        }
        
        
        // Scale 모듈에 따른 현재 크기 계산        
        if (g_Module[0].Module[3])
        {
            float CurScale = ((g_Module[0].EndScale - g_Module[0].StartScale) * Particle.NormalizedAge + g_Module[0].StartScale);
            Particle.WorldScale.xyz = Particle.WorldInitScale.xyz * CurScale;
        }
        
        // Drag 감속 모듈
        if (g_Module[0].Module[4])
        {
            if (Particle.NormalizedAge < g_Module[0].DestNormalizedAge)
            {
                // 기울기
                // (목적 속력 - 현재 속력) / (목적 NA - 현재 NA)
                float Gradient = (g_Module[0].LimitSpeed - length(Particle.Velocity)) / (g_Module[0].DestNormalizedAge - Particle.NormalizedAge);
                float NADT = EngineDT / Particle.Life;
            
                float NewSpeed = length(Particle.Velocity) + (Gradient * NADT);
                Particle.Velocity = normalize(Particle.Velocity) * NewSpeed;
            }
        }
                
        // Render Module
        if (g_Module[0].Module[6])
        {
            Particle.Color = (g_Module[0].EndColor - g_Module[0].StartColor) * Particle.NormalizedAge + g_Module[0].StartColor;

            if (g_Module[0].FadeOut)
            {
                float fRatio = saturate(1.f - (Particle.NormalizedAge - g_Module[0].StartRatio) / (1.f - g_Module[0].StartRatio));
                Particle.Color.a = fRatio;
            }
        }
        
        
        
        // 파티클의 수명이 다하면 비활성화 시킨다.
        Particle.Age += EngineDT;        
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = false;
            g_Buffer[_ID.x] = Particle;
            return;
        }        
                
        g_Buffer[_ID.x] = Particle;
    }
}

#endif