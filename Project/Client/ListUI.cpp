#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
	: m_Idx(-1)
{
}

ListUI::~ListUI()
{
}

void ListUI::Init()
{
	m_vecStr.clear();
	m_Idx = -1;
	m_Selected = "";
}

void ListUI::Update_UI()
{
}

void ListUI::Render_UI()
{
	for (size_t i = 0; i < m_vecStr.size(); ++i)
	{
		int Flags = ImGuiTreeNodeFlags_Bullet;

		if (i == m_Idx)
			Flags |= ImGuiTreeNodeFlags_Selected;
		 
		if (ImGui::TreeNodeEx(m_vecStr[i].c_str(), Flags))
		{			
			ImGui::TreePop();
		}
		 
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			m_Idx = i;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			m_Idx = i;
			m_Selected = m_vecStr[m_Idx];
			SetActive(false);

			if (m_UI && m_MemFunc)
			{
				(m_UI->*m_MemFunc)((DWORD_PTR)this);
			}
		}
	}
}

void ListUI::Activate()
{
	Init();
}

void ListUI::Deactivate()
{
	ImGui::SetWindowFocus(nullptr);
}
