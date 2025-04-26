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

	// Tree 옵션 설정
	m_Tree->SetSeperator(false);
	m_Tree->ShowRoot(false);

	// 특정 항목이 선택되었을때 호출시킬 함수(Delegate) 등록
	m_Tree->AddSelectDelegate(this, (EUI_DELEGATE_1)&Outliner::Selection);

	// 트리 내에서 자체적으로 Drag Drop 이 발생할 경우 호출시킬 맴버함수(Delegate) 등록
	m_Tree->AddSelfDragDropDelegate(this, (EUI_DELEGATE_2)&Outliner::DragDrop);
}

void Outliner::Update_UI()
{
	// 현재 레벨에 변경점이 발생한 프레임에는 트리를 다시 구성한다.
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
	// 루트를 제외한 모든 노드를 삭제
	m_Tree->Clear();

	// 루트노드 가져옴
	TreeNode* pRoot = m_Tree->GetRoot();

	// 게임 오브젝트를 가져옴
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
