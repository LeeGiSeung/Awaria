#pragma once
#include "EditorUI.h"

class ListUI :
    public EditorUI
{
private:
    vector<string>  m_vecStr;
    int             m_Idx;
    string          m_Selected;

    EditorUI*       m_UI;
    EUI_DELEGATE_1  m_MemFunc;

public:
    void AddItem(const string& _String) { m_vecStr.push_back(_String); }
    void AddItem(const vector<string>& _vecStr) { m_vecStr.insert(m_vecStr.end(), _vecStr.begin(), _vecStr.end()); }    
    const string& GetSelected() { return m_Selected; }

public:
    void AddDelegate(EditorUI* _UI, EUI_DELEGATE_1 _MemFunc)
    {
        m_UI = _UI;
        m_MemFunc = _MemFunc;
    }


public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    ListUI();
    ~ListUI();
};

