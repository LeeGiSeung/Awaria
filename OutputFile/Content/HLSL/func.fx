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
        // ���� ��ü�� �Ÿ�
        float fDist = distance(g_Light2D[_LightIdx].WorldPos.xy, _PixelWorldPos.xy);
                
        // ������ �Ÿ��� ���� ���� ���� �������� ���Ѵ�. (1 ~ 0)
        if (fDist > g_Light2D[_LightIdx].Radius)
            DistRatio = 0.f;
        else
            DistRatio = saturate(cos(fDist * ((PI * 0.5f) / g_Light2D[_LightIdx].Radius)));
        
        // ���� ����, �Ÿ��� ���� ��������� �����ش�.
        _vLightColor += g_Light2D[_LightIdx].Color.rgb * DistRatio;
    }
    
    // Spot
    else
    {
        // ���� ��ü�� �Ÿ�
        float fDist = distance(g_Light2D[_LightIdx].WorldPos.xy, _PixelWorldPos.xy);
        
        // SpotLight Angle ���� �̳��� �ִ��� �׽�Ʈ�Ѵ�.
        float Angle = g_Light2D[_LightIdx].Angle / 3.f;
        
        // ���� �߽ɿ��� Pixel �� ���ϴ� ���⺤�͸� ���Ѵ�.
        float3 ToPixel = normalize(float3(_PixelWorldPos.xy - g_Light2D[_LightIdx].WorldPos.xy, 0.f));   
        
        // ������ ���ϴ� �����, �������� ���� ���� �ȼ��� ���ϴ� ���� ������ ���� ���ϱ�
        float CosTheta = dot(ToPixel, g_Light2D[_LightIdx].Dir);        
        float Theta = acos(CosTheta);
        
        if (Angle < Theta || g_Light2D[_LightIdx].Radius < fDist)
        {
            DistRatio = 0.f;
        }
        else
        {
            // ������ �Ÿ��� ���� ���� ���� �������� ���Ѵ�. (1 ~ 0)
            DistRatio = saturate(cos(fDist * ((PI * 0.5f) / g_Light2D[_LightIdx].Radius)));
        }
        
        // ���� ����, �Ÿ��� ���� ��������� �����ش�.
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