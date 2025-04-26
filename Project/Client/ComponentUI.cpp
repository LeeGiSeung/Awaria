#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

ComponentUI::ComponentUI(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Target(nullptr)
	, m_ItemHeight(0.f)
{
	SetName(COMPONENT_TYPE_STR[(UINT)m_Type]);

	SetSeperator(true);

	SetActive(false);
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::OutputTitle(string _Title)
{	
	m_ItemHeight = 0.f;

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button,			ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered,	ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,	ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	
	if(_Title == "")
		ImGui::Button(GetName().c_str());
	else
		ImGui::Button(_Title.c_str());
	AddItemHeight();

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Dummy(ImVec2(0.f, 10.f));
	AddItemHeight();
}

void ComponentUI::SetTarget(CGameObject* _Target)
{
	m_Target = _Target;

	// Target 이 nullptr 이거나,  ComponentUI 가 담당하는 Component 를 GameObject 가 가지고 있지 않다면
	if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_Type))
	{
		SetActive(false);
	}
	else
	{
		SetActive(true);
	}
}

void ComponentUI::Update_UI()
{
	SetSizeAsChild(ImVec2(0.f, m_ItemHeight));
}

void ComponentUI::AddItemHeight()
{
	ImVec2 vSize = ImGui::GetItemRectSize();
	m_ItemHeight += vSize.y + 3.f;
}
