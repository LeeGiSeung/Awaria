#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CTaskMgr.h>

#include "TreeUI.h"

#include "ImGuiMgr.h"
#include "Inspector.h"


Outliner::Outliner()
	: m_Tree(nullptr)
{
}

Outliner::~Outliner()
{
}

void Outliner::Init()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	AddChildUI(m_Tree);

	// Tree �ɼ� ����
	m_Tree->SetSeperator(false);
	m_Tree->ShowRoot(false);

	// Ư�� �׸��� ���õǾ����� ȣ���ų �Լ�(Delegate) ���
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&Outliner::Selection);

	// Ʈ�� ������ ��ü������ Drag Drop �� �߻��� ��� ȣ���ų �ɹ��Լ�(Delegate) ���
	m_Tree->AddSelfDragDropDelegate(this, (EUI_DELEGATE_2)&Outliner::DragDrop);
}

void Outliner::Update_UI()
{
	// ���� ������ �������� �߻��� �����ӿ��� Ʈ���� �ٽ� �����Ѵ�.
	if (CTaskMgr::GetInst()->IsLevelChanged())
	{
		Renew();
	}
}

void Outliner::Render_UI()
{

}

void Outliner::Renew()
{
	// ��Ʈ�� ������ ��� ��带 ����
	m_Tree->Clear();

	// ��Ʈ��� ������
	TreeNode* pRoot = m_Tree->GetRoot();

	// ���� ������Ʈ�� ������
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	assert(pLevel);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{			
			AddGameObject(pRoot, vecObj[j]);
		}
	}
}

void Outliner::Selection(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;

	string Name = pNode->GetName();
	CGameObject* pObject = (CGameObject*)pNode->GetData();

	if (nullptr == pObject)
		return;

	Inspector* pInspector = ImGuiMgr::GetInst()->FindUI<Inspector>("Inspector");
	pInspector->SetTargetObject(pObject);
}

void Outliner::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _TargetNode)
{
	TreeNode* pDragNode = (TreeNode*)_DragNode;
	TreeNode* pDropNode = (TreeNode*)_TargetNode;

	CGameObject* pChild = (CGameObject*)pDragNode->GetData();

	CGameObject* pParent = nullptr;	
	if(nullptr != pDropNode)
		pParent = (CGameObject*)pDropNode->GetData();

	if (pParent)
	{
		if (pParent->IsAncestor(pChild) || pParent == pChild->GetParent())
		{
			return;
		}
	}	
	
	tTask task = {};
	task.Type = TASK_TYPE::ADD_CHILD;
	task.Param0 = (DWORD_PTR)pParent;
	task.Param1 = (DWORD_PTR)pChild;
	CTaskMgr::GetInst()->AddTask(task);
}

void Outliner::AddGameObject(TreeNode* _ParentNode, CGameObject* _Object)
{
	TreeNode* pNode = m_Tree->AddItem( _ParentNode
						, string(_Object->GetName().begin(), _Object->GetName().end())
						, (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pNode, vecChild[i]);
	}
}
