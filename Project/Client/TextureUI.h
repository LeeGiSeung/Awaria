#pragma once
#include "AssetUI.h"
class TextureUI :
    public AssetUI
{

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    TextureUI();
    ~TextureUI();
};

