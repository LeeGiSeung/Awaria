#pragma once
#include "EditorUI.h"
#include <array>

class CFlipbook;

class ObjectEditor :
    public EditorUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    void ShowCollisionTable();


    Ptr<CFlipbook>          pFlipbook = nullptr;
    bool                    Mesh = false;
    int                     FlipbookListIndex;
    int                     LayerInt;
    int                     LayerPreInt;
    int                     PlaySpeed;
    bool                    Repeat;

    int                     PreFabObjectIndex = - 1;

    vector<Ptr<CFlipbook>>  FlipBookVector;

    std::array<bool, COMPONENT_TYPE::COMPONENT_END> componentFlags;

public:
    ObjectEditor();
    ~ObjectEditor();
};

