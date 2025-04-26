#pragma once
#include "EditorUI.h"

class CTexture;
class CGameObject;

class TileMapEditor :
    public EditorUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    string png = ".png";
    string sprite = "_0.sprite";
    string InputTileName;

    Ptr<CTexture> pTex;
    CGameObject* pObject;
    bool SeeImage;

    int SpriteListIndex = 0;
    int TileObjectIndex = -1;

    wstring wOriginalName;
    wstring wTexturePath;

    int TileWidth = 10;
    int TileHeight = 10;

    float TileWidthSize = 100;
    float TileHeightSize = 100;

public:
    TileMapEditor();
    ~TileMapEditor();
};

