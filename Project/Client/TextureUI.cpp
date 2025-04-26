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

	// Asset 종류
	OutputTitle();

	// Key, Path 정보
	OutputKeyPath();

	// 텍스쳐 	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);
	
	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
			    , ImVec2(150.f, 150.f)			   // 텍스쳐 위젯 크기
				, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
				, tint_col, border_col);		   // 틴트, 보더 컬러

	// 텍스쳐 해상도
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
