#pragma once
#include "ComponentUI.h"

class ScriptUI :
    public ComponentUI
{
private:
    static  UINT    ScriptUINextID;
    class CScript*  m_TargetScript;
    UINT            m_ScriptUI_ID;
    

public:
    void SetScript(CScript* _Script);
    CScript* GetScript() { return m_TargetScript; }

public:   
    virtual void Render_UI() override;

public:
    ScriptUI();
    ~ScriptUI();
};

