#pragma once
#include "ComponentUI.h"

class TransformUI :
    public ComponentUI
{
private:

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    TransformUI();
    ~TransformUI();
};

