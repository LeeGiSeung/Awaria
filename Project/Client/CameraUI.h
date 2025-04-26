#pragma once

#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{


public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    CameraUI();
    ~CameraUI();
};

