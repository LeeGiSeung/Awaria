#pragma once
#include "EditorUI.h"

class CFlipbook;

class FlipBookEditor :
    public EditorUI
{

private:
    Ptr<CFlipbook>          m_CurFlipbook;
    int                     FlipbookListIndex;
    int                     Cur_FlipbookListIndex;
    int                     FlipbookListIndex0;
    int                     SpriteListIndex;
    int                     m_Offset_x;
    int                     seem_Offset_x;
    int                     seem_Offset_y;
    int                     m_Offset_y;
    int                     m_SelectIndex;
    int                     m_PosetSelectIndex;

    float LeftTop_x;
    float LeftTop_y;
    float Slice_x;
    float Slice_y;

    void LoadAllFlipbookFiles();
    void LoadAllSpriteFiles();

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    FlipBookEditor();
    ~FlipBookEditor();
};


