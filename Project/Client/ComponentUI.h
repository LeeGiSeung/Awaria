#pragma once
#include "EditorUI.h"

class CGameObject;

class ComponentUI :
    public EditorUI
{
private:
    const COMPONENT_TYPE    m_Type; // 담당 컴포넌트 타입
    CGameObject*            m_Target;    
    float                   m_ItemHeight;

protected:
    void OutputTitle(string _Title = "");
    void AddItemHeight();

public:
    void SetTarget(CGameObject* _Target);
    CGameObject* GetTarget() { return m_Target; }

    virtual void Update_UI() override;

public:
    ComponentUI(COMPONENT_TYPE _Type);
    ~ComponentUI();
};

