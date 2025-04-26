#pragma once

#include "ComponentUI.h"

class RenderComUI :
    public ComponentUI
{
private:
    
public:
    virtual void Render_UI() override;

private:
    void SelectMesh(DWORD_PTR _Data);
    void SelectMaterial(DWORD_PTR _Data);

public:
    RenderComUI(COMPONENT_TYPE _Type);
    ~RenderComUI();
};

