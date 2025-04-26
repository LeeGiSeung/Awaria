#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
struct VS_IN
{    
    float3 vPos : POSITION; // Semantic, InputLayout
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    // System Value Semantic
    // SV_Position : 레스터라이져에 NDC 좌표를 전달할 때 사용하는 Semantic
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);    
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;   
    
    return output;
}


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    if(g_int_0 == 1)
        discard;
    
    if (g_int_1 == 2)
        discard;
    
    float4 vSampleColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // 출력색상
    if (g_btex_0)    
        vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);    
    
    // 노이즈 텍스쳐
    if(g_btex_1)
    {
        float fNoise = g_tex_1.Sample(g_sam_1, _in.vUV).r;
                
        if (1.f <= fNoise + g_float_0)
        {
            discard;
        }        
    }
          
    if (vSampleColor.a == 0.f)
        discard;
        
    // 광원처리   
    float3 vLightColor = (float3) 0.f;
    CalcLight2D(_in.vWorldPos, vLightColor);
   
    // 물체의 색상 * 광원색상 == 최종 색상
    vSampleColor.rgb = vSampleColor.rgb * vLightColor;
    
    return vSampleColor;
}


float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vSampleColor = g_tex_0.Sample(g_sam_1, _in.vUV);
      
    
    return vSampleColor;
}

#endif