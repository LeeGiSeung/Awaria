#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: AssetUI(TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Update_UI()
{
}

void TextureUI::Render_UI()
{
	Ptr<CTexture> pTex = dynamic_cast<CTexture*>(GetAsset().Get());
	assert(pTex.Get());

	// Asset ����
	OutputTitle();

	// Key, Path ����
	OutputKeyPath();

	// �ؽ��� 	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	
	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // �������� �ؽ��� ShaderResourceView
			    , ImVec2(150.f, 150.f)			   // �ؽ��� ���� ũ��
				, uv_min, uv_max				   // ����� �ؽ��� UV ����
				, tint_col, border_col);		   // ƾƮ, ���� �÷�

	// �ؽ��� �ػ�
	char buff[255] = {};

	int width = pTex->GetWidth();
	int height = pTex->GetHeight();

	sprintf_s(buff, 255, "%d", width);
	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##Width", buff, 255, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, 255, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##Height", buff, 255, ImGuiInputTextFlags_ReadOnly);
}
