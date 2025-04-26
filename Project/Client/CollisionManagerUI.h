#pragma once
#include "EditorUI.h"
class CollisionManagerUI :
    public EditorUI
{
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    CollisionManagerUI();
    ~CollisionManagerUI();
};

