#pragma once
#include "AssetUI.h"

class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM   m_TargetTex;


public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void Material();

    void Shader();

    void ScalarParameter();
    void TextureParameter();



private:
    void SelectShader(DWORD_PTR _Data);
    void SelectTexture(DWORD_PTR _Data);

public:
    MaterialUI();
    ~MaterialUI();
};

