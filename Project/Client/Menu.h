#pragma once
#include "EditorUI.h"

class CLevel;

class Menu :
    public EditorUI
{
private:
    bool    m_SpriteEditor;
    bool    m_FlipbookEditor;
    bool    m_TileEditor;
    bool    m_ObjectEditor;
    bool    m_CollisionManagerUI;
    bool    m_CutSceneEditor;
    bool    m_Prefab;

    CLevel* m_CloneLevel;

private:
    void File();
    void Level();
    void Editor();
    void Script();
    void Asset();

    string GetMaterialName();


public:
    virtual void Render() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    Menu();
    ~Menu();
};

