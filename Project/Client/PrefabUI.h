#pragma once
#include "AssetUI.h"
class PrefabUI :
    public AssetUI
{
public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    PrefabUI();
    ~PrefabUI();
};

