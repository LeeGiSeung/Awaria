#pragma once
#include "AssetUI.h"
class FlipbookUI :
    public AssetUI
{
public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    FlipbookUI();
    ~FlipbookUI();
};

