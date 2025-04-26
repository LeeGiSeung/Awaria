#include "pch.h"
#include "PrefabUI.h"

void PrefabUI::Update_UI()
{
}

void PrefabUI::Render_UI()
{
	OutputTitle();
}

PrefabUI::PrefabUI()
	: AssetUI(PREFAB)
{
}

PrefabUI::~PrefabUI()
{
}
