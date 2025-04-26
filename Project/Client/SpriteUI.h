#pragma once
#include "AssetUI.h"
class SpriteUI :
    public AssetUI
{
public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    SpriteUI();
    ~SpriteUI();
};

