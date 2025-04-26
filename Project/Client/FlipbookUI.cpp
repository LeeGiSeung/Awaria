#include "pch.h"
#include "FlipbookUI.h"

void FlipbookUI::Update_UI()
{
}

void FlipbookUI::Render_UI()
{
	OutputTitle();
	ImGui::Text("faefa");
}

FlipbookUI::FlipbookUI()
	: AssetUI(FLIPBOOK)
{
}

FlipbookUI::~FlipbookUI()
{
}
