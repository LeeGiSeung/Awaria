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

	// ���ϸ� ���̰� �� ���, ��ι��ڿ����� �̸��� �����ؼ� ���̰� ��
	if (Tree->IsShowOnlyName())
	{
		name = path(Name).stem().string();
	}

	// ������ ����ε�, �ڽ��� ���� ���
	if (Frame)
	{
		Flag |= ImGuiTreeNodeFlags_Framed;

		if (vecChild.empty())
		{
			name = "   " + name;
		}
	}

	// ��� �̸� �ڿ� ���� ID ���̱�
	char buff[20] = {};
	_itoa_s(ID, buff, 20, 10);
	name = name + "##" + buff;
	 
	if (Tree->GetSelectedNode() == this)
		Flag |= ImGuiTreeNodeFlags_Selected;

	bool bSelected = false;

	if (ImGui::TreeNodeEx(name.c_str(), Flag))
	{
		// ��尡 ������ ������ �� Ŭ���Ǿ����� Ȯ��
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) 
			&& ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			bSelected = true;
		}

		// �巡�� üũ
		DragCheck();

		// ��� üũ
		DropCheck();

		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			vecChild[i]->Render_UI();
		}

		ImGui::TreePop();
	}

	else
	{
		// ��尡 �����ִ� �����϶� Ŭ���Ǿ����� Ȯ��
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)
			&& ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			bSelected = true;
		}

		// �巡�� üũ
		DragCheck();

		// ��� üũ
		DropCheck();
	}
	
	if (bSelected)
	{
		Tree->SetSelectedNode(this);
	}
}

void TreeNode::DragCheck()
{
	// �ش� Ʈ����尡 �巡�� ������ Ȯ��
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		// Payload ���
		// Node �� ������ ��ü(TreeUI) �� �̸��� Payload Ű������ ����Ѵ�.
		// ������ �����ʹ� Node �� ����ִ� Data �̴�.
		ImGui::SetDragDropPayload(Tree->GetName().c_str(), &Data, sizeof(DWORD_PTR));

		// ���콺�� ����ٴϸ鼭 � �׸��� �巡���Ѱ��� ������ �����ִ� �뵵
		ImGui::Text(Name.c_str());

		// Tree ���� �ڽ��� Drag ���� ����� ���� �˸�
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
		// ContentUI ���� �巡�׵� �����͸� �޴´�.
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

	// �⺻���� ��Ʈ��� �ϳ� �����ص�
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

	// ���õ� ��尡 ������ �߻��ߴٸ�
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
		//Prefab ��Ŭ��
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
