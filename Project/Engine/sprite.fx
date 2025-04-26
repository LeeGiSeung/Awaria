#ifndef _SPRITE
#define _SPRITE

#include "value.fx"

#define ATLAS_TEX g_tex_0
#define LeftTop   g_vec2_0
#define Slice     g_vec2_1

struct VS_IN
{
    float3 vPos : POSITION; // Semantic, InputLayout   
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position; 
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Sprite(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP); 
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Sprite(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
     
    if(1 == g_int_0)
    {
        vColor = ATLAS_TEX.Sample(g_sam_1, LeftTop + (Slice * _in.vUV));
    }
    
    if (0.f == vColor.a)
        discard;    
    
    return vColor;
}

#endif