#ifndef _FUNC
#define _FUNC

#include "value.fx"


int IsBind(Texture2D _tex)
{
    uint width = 0;
    uint height = 0;
    
    _tex.GetDimensions(width, height);
    
    if (width || height)
        return true;    
    else
        return false;    
}


void CalcLight2D(int _LightIdx, float3 _PixelWorldPos, inout float3 _vLightColor)
{          
    float DistRatio = 1.f;
    
    // Directional
    if (g_Light2D[_LightIdx].Type == 0)
    {
        _vLightColor += g_Light2D[_LightIdx].Color.rgb;
    }
    
    // Point
    else if (g_Light2D[_LightIdx].Type == 1)
    {
        // 빛과 물체의 거리
        float fDist = distance(g_Light2D[_LightIdx].WorldPos.xy, _PixelWorldPos.xy);
                
        // 빛과의 거리에 따른 빛의 세기 비율값을 구한다. (1 ~ 0)
        if (fDist > g_Light2D[_LightIdx].Radius)
            DistRatio = 0.f;
        else
            DistRatio = saturate(cos(fDist * ((PI * 0.5f) / g_Light2D[_LightIdx].Radius)));
        
        // 빛의 색상에, 거리에 따른 감쇠비율을 곱해준다.
        _vLightColor += g_Light2D[_LightIdx].Color.rgb * DistRatio;
    }
    
    // Spot
    else
    {
        // 빛과 물체의 거리
        float fDist = distance(g_Light2D[_LightIdx].WorldPos.xy, _PixelWorldPos.xy);
        
        // SpotLight Angle 범위 이내에 있는지 테스트한다.
        float Angle = g_Light2D[_LightIdx].Angle / 3.f;
        
        // 광원 중심에서 Pixel 을 향하는 방향벡터를 구한다.
        float3 ToPixel = normalize(float3(_PixelWorldPos.xy - g_Light2D[_LightIdx].WorldPos.xy, 0.f));   
        
        // 광원이 향하는 방향과, 광원에서 빛을 받을 픽셀을 향하는 방향 사이의 각도 구하기
        float CosTheta = dot(ToPixel, g_Light2D[_LightIdx].Dir);        
        float Theta = acos(CosTheta);
        
        if (Angle < Theta || g_Light2D[_LightIdx].Radius < fDist)
        {
            DistRatio = 0.f;
        }
        else
        {
            // 빛과의 거리에 따른 빛의 세기 비율값을 구한다. (1 ~ 0)
            DistRatio = saturate(cos(fDist * ((PI * 0.5f) / g_Light2D[_LightIdx].Radius)));
        }
        
        // 빛의 색상에, 거리에 따른 감쇠비율을 곱해준다.
        _vLightColor += g_Light2D[_LightIdx].Color.rgb * DistRatio;        
    }
}

void CalcLight2D(float3 _PixelWorldPos, inout float3 _vLightColor)
{
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _PixelWorldPos, _vLightColor);
    }
}

#endif