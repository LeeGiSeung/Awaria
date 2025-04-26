#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"

#define OBJECT_POS g_vec4_0

StructuredBuffer<tParticle> m_Buffer : register(t20);
StructuredBuffer<tParticleModule> m_Module : register(t21);



struct VS_IN
{    
    uint InstID : SV_InstanceID;
};


struct VS_OUT
{
    float InstID : FOG;
};

VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.InstID = _in.InstID;      
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;    
    float InstID : FOG;
};

// 1. 빌보드 구현하기
// 2. 비활성화 된 파티클을 건너뛰기
[maxvertexcount(12)]
void GS_Particle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _Stream)
{
    uint InstID = (uint)_in[0].InstID;    
    if (false == m_Buffer[InstID].Active)
        return;
        
    GS_OUT output[4]        = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    GS_OUT output_cross[4]  = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    float3 vWorldPos = m_Buffer[InstID].WorldPos;
       
    float3 vViewPos = mul(float4(vWorldPos, 1.f), g_matView).xyz;    
    
    float3 vScale = m_Buffer[InstID].WorldScale.xyz;
    
    output[0].vPosition.xyz = float3(-vScale.x / 2.f, +vScale.y / 2.f, 0.f);
    output[1].vPosition.xyz = float3(+vScale.x / 2.f, +vScale.y / 2.f, 0.f);
    output[2].vPosition.xyz = float3(+vScale.x / 2.f, -vScale.y / 2.f, 0.f);
    output[3].vPosition.xyz = float3(-vScale.x / 2.f, -vScale.y / 2.f, 0.f);
       
    if (m_Module[0].CrossMesh)
    {
        output_cross[0].vPosition.xyz = float3(-vScale.x / 2.f, 0.f, +vScale.y / 2.f);
        output_cross[1].vPosition.xyz = float3(+vScale.x / 2.f, 0.f, +vScale.y / 2.f);
        output_cross[2].vPosition.xyz = float3(+vScale.x / 2.f, 0.f, -vScale.y / 2.f);
        output_cross[3].vPosition.xyz = float3(-vScale.x / 2.f, 0.f, -vScale.y / 2.f);
    }
    
    // 속도 정렬(진행 방향으로 회전시키기, 빌보드 X)
    if (m_Module[0].VelocityAlignment)
    {        
        float3 vR = normalize(mul(float4(m_Buffer[InstID].Velocity.xyz, 0.f), g_matView).xyz);
        float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
        float3 vU = normalize(cross(vF, vR));

        float3x3 vRot =
        {
            vR,
            vU,
            vF,
        };
        
        for (int i = 0; i < 4; ++i)
        {
            output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
            
            if (m_Module[0].CrossMesh)
            {
                output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
            }            
        }
    }
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(float4(output[i].vPosition.xyz, 1.f), g_matProj);
        
        if(m_Module[0].CrossMesh)
        {
            output_cross[i].vPosition.xyz += vViewPos.xyz;
            output_cross[i].vPosition = mul(float4(output_cross[i].vPosition.xyz, 1.f), g_matProj);
            output_cross[i].InstID = _in[0].InstID;

        }        
        
        output[i].InstID = _in[0].InstID;
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    if(m_Module[0].CrossMesh)
    {
        output_cross[0].vUV = float2(0.f, 0.f);
        output_cross[1].vUV = float2(1.f, 0.f);
        output_cross[2].vUV = float2(1.f, 1.f);
        output_cross[3].vUV = float2(0.f, 1.f);
    }
    
    _Stream.Append(output[0]);
    _Stream.Append(output[1]);
    _Stream.Append(output[2]);
    _Stream.RestartStrip();

    _Stream.Append(output[0]);
    _Stream.Append(output[2]);
    _Stream.Append(output[3]);
    _Stream.RestartStrip();
    
    if(m_Module[0].CrossMesh)
    {
        _Stream.Append(output_cross[0]);
        _Stream.Append(output_cross[1]);
        _Stream.Append(output_cross[2]);
        _Stream.RestartStrip();

        _Stream.Append(output_cross[0]);
        _Stream.Append(output_cross[2]);
        _Stream.Append(output_cross[3]);
        _Stream.RestartStrip();
    } 
}

float4 PS_Particle(GS_OUT _in) : SV_Target
{    
    //float4 vColor = m_Buffer[_in.InstID].Color;
    //vColor.a = 1.f;
    //return vColor;
    
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    vColor *= m_Buffer[_in.InstID].Color;
    return vColor;
}

#endif