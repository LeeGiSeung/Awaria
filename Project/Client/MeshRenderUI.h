#pragma once
#include "RenderComUI.h"
class MeshRenderUI :
    public RenderComUI
{

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

