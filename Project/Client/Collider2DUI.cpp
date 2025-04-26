#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Update_UI()
{
}

void Collider2DUI::Render_UI()
{
	CGameObject* Target = GetTarget();
	CCollider2D* pCollider = Target->Collider2D();

	OutputTitle();

	Vec2 vScaleScale = pCollider->GetScale();
	Vec2 vOffset= pCollider->GetOffset();

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##vScaleScale", vScaleScale))
	{
		pCollider->SetScale(vScaleScale);
	}

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat2("##vOffset", vOffset))
	{
		pCollider->SetOffset(vOffset);
	}

}