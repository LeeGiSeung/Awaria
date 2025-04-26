#pragma once
#include "EditorUI.h"
class CutSceneEditor :
    public EditorUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    CutSceneEditor();
    ~CutSceneEditor();
};

