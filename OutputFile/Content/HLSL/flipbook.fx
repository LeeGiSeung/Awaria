#ifndef _FLIPBOOK
#define _FLIPBOOK

#include "value.fx"

#define ATLAS_TEX   g_tex_0

#define LeftTop     g_vec2_0
#define Slice       g_vec2_1
#define Background  g_vec2_2
#define Offset      g_vec2_3
#define FlipDir     g_int_1 // 0: Left(Default), 1: Right(Flip)

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

VS_OUT VS_Flipbook(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
                
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Flipbook(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    if (1 == g_int_0)
    {        
        float2 vUV = LeftTop + (Slice - Background) * 0.5f + (Background * _in.vUV) - Offset;
        
        // 좌우 반전 적용
        if (1 == FlipDir)
        {
            vUV.x = LeftTop.x + Slice.x - (vUV.x - LeftTop.x);
        }
        
        if (vUV.x < LeftTop.x || (LeftTop.x + Slice.x) < vUV.x
            || vUV.y < LeftTop.y || (LeftTop.y + Slice.y) < vUV.y)
        {
            discard;
        }        
        
       vColor = ATLAS_TEX.Sample(g_sam_1, vUV);
        
    }
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}


#endif