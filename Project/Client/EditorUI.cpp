#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI()
	: m_Active(true)
	, m_ParentUI(nullptr)
	, m_ChildSize(ImVec2(0.f, 0.f))
	, m_Modal(false)
	, m_Seperator(false)
	, m_Focused(false)
{
}

EditorUI::~EditorUI()
{
	Delete_Vec(m_vecChildUI);
}

void EditorUI::Update()
{	
	Update_UI();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Update();
	}
}

void EditorUI::Render()
{
	if (!m_Active)
		return;

	


	// 최상위 부모 UI 인 경우
	if (nullptr == m_ParentUI)
	{
		// Modalless
		if (!m_Modal)
		{
			ImGui::Begin(m_Name.c_str(), &m_Active);

			CheckFocus();

			Render_UI();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Render();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(m_Name.c_str());
			if (ImGui::BeginPopupModal(m_Name.c_str(), &m_Active))
			{
				Render_UI();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Render();
				}

				ImGui::EndPopup();
			}
		}
	}

	// 자식 UI  인 경우
	else
	{
		ImGui::BeginChild(m_Name.c_str(), m_ChildSize);

		// 포커싱 체크
		CheckFocus();

		Render_UI();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Render();
		}

		ImGui::EndChild();

		// 구분선
		if(m_Seperator)
			ImGui::Separator();
	}

	
}

void EditorUI::SetActive(bool _Active)
{
	if (m_Active == _Active)
		return;

	m_Active = _Active;

	if (m_Active)
		Activate();
	else
		Deactivate();
}

void EditorUI::CheckFocus()
{
	// 현재 포커싱 중이라면
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows))
	{
		// 이전에는 포커싱이 아니였다면
		if (false == m_Focused)
		{
			OnFocus();
		}

		m_Focused = true;
	}

	// 현재 포커싱중이 아니라면
	else
	{
		// 이전 프레임에는 Focucing 중이었다면
		if (true == m_Focused)
		{
			OffFocus();
		}

		m_Focused = false;
	}
}