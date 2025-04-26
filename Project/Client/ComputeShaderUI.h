#pragma once
#include "AssetUI.h"
class ComputeShaderUI :
    public AssetUI
{
public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};

