#include "pch.h"
#include "TreeUI.h"

// ========
// TreeNode
// ========
UINT TreeNode::g_NextID = 0;

TreeNode::TreeNode()
	: Data(0)
	, ID(g_NextID++)
	, Tree(nullptr)
	, Parent(nullptr)
	, Frame(false)
{
}

TreeNode::TreeNode(const string& _Name, DWORD_PTR _Data)
	: Name(_Name)
	, Data(_Data)
	, ID(g_NextID++)
	, Tree(nullptr)
	, Parent(nullptr)
	, Frame(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(vecChild);
}

void TreeNode::Render_UI()
{
	int Flag = ImGuiTreeNodeFlags_OpenOnDoubleClick;
	string name = Name;

	if (vecChild.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;

	// 파일명만 보이게 한 경우, 경로문자열에서 이름만 추출해서 보이게 함
	if (Tree->IsShowOnlyName())
	{
		name = path(Name).stem().string();
	}

	// 프레임 노드인데, 자식이 없는 경우
	if (Frame)
	{
		Flag |= ImGuiTreeNodeFlags_Framed;

		if (vecChild.empty())
		{
			name = "   " + name;
		}
	}

	// 출력 이름 뒤에 고유 ID 붙이기
	char buff[20] = {};
	_itoa_s(ID, buff, 20, 10);
	name = name + "##" + buff;
	 
	if (Tree->GetSelectedNode() == this)
		Flag |= ImGuiTreeNodeFlags_Selected;

	bool bSelected = false;

	if (ImGui::TreeNodeEx(name.c_str(), Flag))
	{
		// 노드가 펼쳐진 상태일 때 클릭되었는지 확인
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) 
			&& ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			bSelected = true;
		}

		// 드래그 체크
		DragCheck();

		// 드랍 체크
		DropCheck();

		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->Render_UI();
		}

		ImGui::TreePop();
	}

	else
	{
		// 노드가 닫혀있는 상태일때 클릭되었는지 확인
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)
			&& ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			bSelected = true;
		}

		// 드래그 체크
		DragCheck();

		// 드랍 체크
		DropCheck();
	}
	
	if (bSelected)
	{
		Tree->SetSelectedNode(this);
	}
}

void TreeNode::DragCheck()
{
	// 해당 트리노드가 드래그 중인지 확인
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// Payload 등록
		// Node 를 소유한 본체(TreeUI) 의 이름을 Payload 키값으로 사용한다.
		// 전달할 데이터는 Node 가 들고있는 Data 이다.
		ImGui::SetDragDropPayload(Tree->GetName().c_str(), &Data, sizeof(DWORD_PTR));

		// 마우스를 따라다니면서 어떤 항목을 드래그한건지 정보를 보여주는 용도
		ImGui::Text(Name.c_str());

		// Tree 에게 자신이 Drag 중인 노드라는 것을 알림
		Tree->SetDragNode(this);

		ImGui::EndDragDropSource();
	}
}

void TreeNode::DropCheck()
{
	if (!Tree->IsSelfDragDrop())
		return;

	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI 에서 드래그된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Tree->GetName().c_str());

		if (nullptr != payload)
		{
			Tree->SetDropNode(this);
		}

		ImGui::EndDragDropTarget();
	}
}

// ======
// TreeUI
// ======
TreeUI::TreeUI()
	: m_Root(nullptr)
	, m_ShowRoot(false)
	, m_SelectedInst(nullptr)
	, m_SelectedFunc(nullptr)
	, m_SelectedChange(nullptr)
	, m_ShowOnlyName(false)
	, m_DragNode(nullptr)
	, m_DropNode(nullptr)
	, m_SDDInst(nullptr)
	, m_SDDFunc(nullptr)
	, m_SelfDragDrop(false)
{
	SetName("TreeUI");

	// 기본적인 루트노드 하나 생성해둠
	m_Root = new TreeNode("Root");	
	m_Root->Tree = this;
}

TreeUI::~TreeUI()
{
	if(nullptr != m_Root)
		delete m_Root;
}

void TreeUI::Clear()
{
	if (nullptr != m_Root)
	{
		Delete_Vec(m_Root->vecChild);
	}
}

void TreeUI::Update_UI()
{
}

void TreeUI::Render_UI()
{
	if (nullptr == m_Root)
		return;

	if (m_ShowRoot)
	{
		m_Root->Render_UI();
	}

	else
	{
		for (size_t i = 0; i < m_Root->vecChild.size(); ++i)
		{
			m_Root->vecChild[i]->Render_UI();
		}
	}

	// 선택된 노드가 변경이 발생했다면
	if (m_SelectedChange)
	{
		m_SelectedChange = false;

		if (m_SelectedInst && m_SelectedFunc)
		{
			(m_SelectedInst->*m_SelectedFunc)((DWORD_PTR)m_Selected);
		}
	}	 

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (m_DragNode)
		{
			if (m_DragNode != m_DropNode && (m_SDDInst && m_SDDFunc))
			{
				(m_SDDInst->*m_SDDFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
			}
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		//Prefab 우클릭
		int a = 0;

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}
}

void TreeUI::OffFocus()
{
	SelectionClear();
}

TreeNode* TreeUI::AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data)
{
	TreeNode* pNewNode = new TreeNode(_Name, _Data);
	pNewNode->Tree = this;

	_ParentNode->vecChild.push_back(pNewNode);

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _Node)
{	
	if (m_Selected == _Node)
		return;

	m_Selected = _Node;
	m_SelectedChange = true;
}
