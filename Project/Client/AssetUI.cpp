#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_AssetType(_Type)
{	
	SetName(ASSET_TYPE_STR[m_AssetType]);
}

AssetUI::~AssetUI()
{

}

void AssetUI::SetAsset(Ptr<CAsset> _Asset)
{	
	// 담당하는 Asset 과 다른타입의 Asset 이 입력으로 들어온 경우
	if (nullptr == _Asset || (m_AssetType != _Asset->GetType()))
		SetActive(false);
	else
	{
		SetActive(true);
		m_Asset = _Asset;
	}
}

void AssetUI::OutputTitle()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 0.8f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.4f, 0.8f, 1.f));

	ImGui::Button(GetName().c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Spacing();
	ImGui::Spacing();
}

void AssetUI::OutputKeyPath()
{
	string Key = string(m_Asset->GetKey().begin(), m_Asset->GetKey().end());
	string Path = string(m_Asset->GetRelativePath().begin(), m_Asset->GetRelativePath().end());

	ImGui::Text("Key");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetKey", (char*)Key.c_str(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("Path");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetPath", (char*)Path.c_str(), Path.length() + 1, ImGuiInputTextFlags_ReadOnly);
}
