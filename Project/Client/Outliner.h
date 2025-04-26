#pragma once
#include "EditorUI.h"



class Outliner :
    public EditorUI
{
private:
    class TreeUI* m_Tree;

private:
    void Renew();
    void Selection(DWORD_PTR _Node);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _TargetNde);

    void AddGameObject(struct TreeNode* _ParentNode, class CGameObject* _Object);


public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    Outliner();
    ~Outliner();
};

