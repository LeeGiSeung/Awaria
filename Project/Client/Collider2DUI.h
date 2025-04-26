#pragma once
#include "ComponentUI.h"

class Collider2DUI :
    public ComponentUI
{

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    Collider2DUI();
    ~Collider2DUI();
};

