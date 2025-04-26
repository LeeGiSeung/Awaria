#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_PostProcess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VS_OUT _in) : SV_Target
{
    float2 vUV = _in.vUV;
    
    // ������ �ؽ��İ� �ִٸ�
    if(g_btex_1)
    {
        float3 vNoise = g_tex_1.Sample(g_sam_0, vUV + float2(g_EngineTime * 0.1f, 0.f)).rgb;
        float2 fNoise = vNoise.xy - 0.5f; // -0.5 ~ 0.5f;        
        fNoise *= 0.05f;
        vUV += fNoise;        
        
        if (vUV.x < 0.f || 1.f < vUV.x 
            || vUV.y < 0.f || 1.f < vUV.y)
        {
            vUV = _in.vUV;
        }
    }
    
    float4 vOutColor = PostProcessTarget.Sample(g_sam_1, vUV);
    // ��� ȿ��
    //vOutColor.rgb = (vOutColor.r + vOutColor.g + vOutColor.b) / 3.f;
    
    return vOutColor;
}

VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    // _in.vPosition.xy : �ȼ� ��ǥ
    // Screen UV
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float2 vUV = vScreenUV;
    
    // ������ �ؽ��İ� �ִٸ�
    if (g_btex_1)
    {
        float3 vNoise = g_tex_1.Sample(g_sam_0, vUV + float2(g_EngineTime * 0.1f, 0.f)).rgb;
        float2 fNoise = vNoise.xy - 0.5f; // -0.5 ~ 0.5f;        
        fNoise *= 0.02f;
        vUV += fNoise;
        
        if (vUV.x < 0.f || 1.f < vUV.x 
            || vUV.y < 0.f || 1.f < vUV.y)
        {
            vUV = _in.vUV;
        }
    }
    
    float4 vOutColor = PostProcessTarget.Sample(g_sam_1, vUV);
    vOutColor.r *= 1.2f;
    // ��� ȿ��
    //vOutColor.rgb = (vOutColor.r + vOutColor.g + vOutColor.b) / 3.f;
    
    return vOutColor;
}


#endif