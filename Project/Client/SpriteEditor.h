#pragma once
#include "EditorUI.h"
class SpriteEditor :
    public EditorUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    int MouseSplitCount = 0;

    wstring wOriginalName;
    wstring wTexturePath;

    ImVec2 RedRectLeftTop;

    int curSpriteIndex;
    float LeftTop_x;
    float LeftTop_y;
    float Slice_x;
    float Slice_y;

    bool autoSet = false;
    bool SeeOriginalImage;

public:
    SpriteEditor();
    ~SpriteEditor();
};
