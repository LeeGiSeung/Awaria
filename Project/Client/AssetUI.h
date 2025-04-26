#pragma once
#include "EditorUI.h"

#include <Engine/CAsset.h>

class AssetUI :
    public EditorUI
{
private:
    const ASSET_TYPE    m_AssetType;
    Ptr<CAsset>         m_Asset;

public:
    void SetAsset(Ptr<CAsset> _Asset);
    Ptr<CAsset> GetAsset() { return m_Asset; }

protected:
    void OutputTitle();
    void OutputKeyPath();




public:
    AssetUI(ASSET_TYPE _Type);
    ~AssetUI();
};

