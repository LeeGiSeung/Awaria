#pragma once
#include "CAsset.h"

#include "CTexture.h"

class CSprite :
    public CAsset
{
private:
    Ptr<CTexture>   m_Atlas;
    Vec2            m_LeftTop;
    Vec2            m_Slice;
    Vec2            m_Offset;
    Vec2            m_Background;
    Vec2            m_intOffset;
    Vec2            m_intLeftTop;
    Vec2            m_intSlice;

public:



    void SetAtlasTex(Ptr<CTexture> _Tex) { m_Atlas = _Tex; }
    void SetLeftTop(Vec2 _LeftTopPixel);
    void SetSlice(Vec2 _SlicePixel);
    void SetOffset(Vec2 _OffsetPixel);
    void SetBackground(Vec2 _BackgroundPixel);

    Ptr<CTexture> GetAtlasTex() { return m_Atlas; }
    Vec2 GetLeftTop() { return m_LeftTop; }
    Vec2 GetSlice() { return m_Slice; }
    Vec2 GetOffset() { return m_Offset; }
    Vec2 GetBackground() { return m_Background; }   
    Vec2 GetIntOffset() { return m_intOffset; }

    float denormalize(float normalizedValue, float min, float max) {
        return normalizedValue * (max - min) + min;
    }

    float normalizea(float value, float min, float max) {
        if (max == min) {

            return 0.0f; // 오류 처리
        }
        return (value - min) / (max - min);
    }

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

public:
    CLONE_DISABLE(CSprite);
public:
    CSprite(bool _EngineAsset = false);
    ~CSprite();
};

