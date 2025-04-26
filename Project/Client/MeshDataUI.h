#pragma once
#include "AssetUI.h"
class MeshDataUI :
    public AssetUI
{

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    MeshDataUI();
    ~MeshDataUI();
};

