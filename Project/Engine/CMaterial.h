#pragma once
#include "CAsset.h"

#include "CGraphicShader.h"
#include "CTexture.h"

class CMaterial :
    public CAsset
{
private:
    Ptr<CGraphicShader> m_Shader;
    Ptr<CTexture>       m_arrTex[TEX_PARAM::TEX_END];
    tMtrlConst          m_Const;

    
public:
    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }

    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& _Data);
    template<typename T>
    T& GetScalarParam(SCALAR_PARAM _Type);

    void SetTexParam(TEX_PARAM _Param, const Ptr<CTexture>& _Tex);
    Ptr<CTexture> GetTexParam(TEX_PARAM _Param) { return m_arrTex[(int)_Param]; }

    void Binding();       

    virtual int Save(const wstring& _FilePath);
    virtual int Load(const wstring& _FilePath);

public:
    CLONE(CMaterial);
    CMaterial(bool _EngineAsset = false);
    ~CMaterial();
};

template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{   
    bool bInput = false;

    switch (_Param)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>)
        {
            m_Const.iArr[_Param - INT_0] = _Data;
            bInput = true;
        }        
    }
        break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
    {
        if constexpr (std::is_same_v<T, float>)
        {
            m_Const.fArr[_Param - FLOAT_0] = _Data;
            bInput = true;
        }
    }
        break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
    {
        if constexpr (std::is_same_v<T, Vec2>)
        {
            m_Const.v2Arr[_Param - VEC2_0] = _Data;
            bInput = true;
        }       
    }
        break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
    {
        if constexpr (std::is_same_v<T, Vec4> || std::is_same_v<T, Vec3>)
        {
            m_Const.v4Arr[_Param - VEC4_0] = _Data;
            bInput = true;
        }       
    }     
        break;
    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3:
    {
        if constexpr (std::is_same_v<T, Matrix>)
        {
            m_Const.matArr[_Param - MAT_0] = _Data;
            bInput = true;
        }       
    }
        break;    
    }

    assert(bInput);
}

template<typename T>
T& CMaterial::GetScalarParam(SCALAR_PARAM _Type)
{
    bool bInput = false;

    switch (_Type)
    {
    case INT_0:
    case INT_1:
    case INT_2:
    case INT_3:
    {
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, unsigned int>)
        {            
            bInput = true;
            return m_Const.iArr[_Type - INT_0];
        }
    }
    break;
    case FLOAT_0:
    case FLOAT_1:
    case FLOAT_2:
    case FLOAT_3:
    {
        if constexpr (std::is_same_v<T, float>)
        {            
            bInput = true;
            return m_Const.fArr[_Type - FLOAT_0];
        }
    }
    break;
    case VEC2_0:
    case VEC2_1:
    case VEC2_2:
    case VEC2_3:
    {
        if constexpr (std::is_same_v<T, Vec2>)
        {            
            bInput = true;
            return m_Const.v2Arr[_Type - VEC2_0];
        }
    }
    break;
    case VEC4_0:
    case VEC4_1:
    case VEC4_2:
    case VEC4_3:
    {
        if constexpr (std::is_same_v<T, Vec4> || std::is_same_v<T, Vec3>)
        {
            bInput = true;
            return m_Const.v4Arr[_Type - VEC4_0];
        }
    }
    break;
    case MAT_0:
    case MAT_1:
    case MAT_2:
    case MAT_3:
    {
        if constexpr (std::is_same_v<T, Matrix>)
        {
            bInput = true;
            return m_Const.matArr[_Type - MAT_0];;
        }
    }
    break;
    }

    assert(bInput);
}
