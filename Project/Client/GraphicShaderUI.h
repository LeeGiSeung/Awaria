#pragma once
#include "AssetUI.h"
class GraphicShaderUI :
    public AssetUI
{
public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    GraphicShaderUI();
    ~GraphicShaderUI();
};

