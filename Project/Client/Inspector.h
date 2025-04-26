#pragma once
#include "EditorUI.h"

#include <Engine/CAsset.h>

class CGameObject;
class ComponentUI;
class ScriptUI;
class AssetUI;

class Inspector :
    public EditorUI
{
private:
    CGameObject*        m_Object;
    ComponentUI*        m_ComUI[COMPONENT_END];
    vector<ScriptUI*>   m_vecScriptUI;

    Ptr<CAsset>         m_Asset;
    AssetUI*            m_AssetUI[ASSET_END];
     
public:
    CGameObject* GetTargetObject() { return m_Object; }
    void SetTargetObject(CGameObject* _Object);

    void SetAsset(Ptr<CAsset> _Asset);

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

