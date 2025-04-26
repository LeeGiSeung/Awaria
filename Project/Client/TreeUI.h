#pragma once
#include "EditorUI.h"


struct TreeNode
{
public:
    static UINT         g_NextID;
private:
    string              Name;
    DWORD_PTR           Data;
    UINT                ID;

    class TreeUI*       Tree;
    TreeNode*           Parent;
    vector<TreeNode*>   vecChild;

    bool                Frame;

public:
    void SetFrame(bool _Frame) { Frame = _Frame; }
    string GetName() { return Name; }
    DWORD_PTR GetData() { return Data; }

private:
    void DragCheck();
    void DropCheck();

public:
    void Render_UI();

public:
    TreeNode();
    TreeNode(const string& _Name, DWORD_PTR _Data = 0);
        
    ~TreeNode();

    friend class TreeUI;
};


class TreeUI :
    public EditorUI
{
private:
    TreeNode*       m_Root;
    bool            m_ShowRoot;

    bool            m_ShowOnlyName;

    TreeNode*       m_Selected;
    EditorUI*       m_SelectedInst;
    EUI_DELEGATE_1  m_SelectedFunc;
    bool            m_SelectedChange;


    TreeNode*       m_DragNode;
    TreeNode*       m_DropNode;

    bool            m_SelfDragDrop;
    EditorUI*       m_SDDInst;
    EUI_DELEGATE_2  m_SDDFunc;
         
public:
    TreeNode* GetRoot() { return m_Root; }
    TreeNode* AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data);

    void SetSelectedNode(TreeNode* _Node);
    TreeNode* GetSelectedNode() { return m_Selected; }

    void SetDragNode(TreeNode* _Node) { m_DragNode = _Node; }
    void SetDropNode(TreeNode* _Node) { m_DropNode = _Node; }

    TreeNode* GetDragNode() { return m_DragNode; }
    TreeNode* GetDropNode() { return m_DropNode; }
     
    void AddSelectDelegate(EditorUI* _Inst, EUI_DELEGATE_1 _MemFunc)
    {
        m_SelectedInst = _Inst;
        m_SelectedFunc = _MemFunc;
    }

    void AddSelfDragDropDelegate(EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
    {
        m_SDDInst = _Inst;
        m_SDDFunc = _MemFunc;
        m_SelfDragDrop = true;
    }

    bool IsSelfDragDrop() { return m_SelfDragDrop; }

    void ShowRoot(bool _Show) { m_ShowRoot = _Show; }
    void ShowOnlyName(bool _Show) { m_ShowOnlyName = _Show; }
    bool IsShowOnlyName() { return m_ShowOnlyName; }

    void SelectionClear() { m_Selected = nullptr; }

    void Clear();

public:
    virtual void Update_UI() override;
    virtual void Render_UI() override;    

    virtual void OffFocus() override;

public:
    TreeUI();
    ~TreeUI();
};

