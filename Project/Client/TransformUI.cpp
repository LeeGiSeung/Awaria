#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM)
{	
}

TransformUI::~TransformUI()
{
}

void TransformUI::Update_UI()
{
}

void TransformUI::Render_UI()
{
	CGameObject* Target = GetTarget();
	CTransform* pTransform = Target->Transform();

	OutputTitle();

	Vec3 vRelativePos		= pTransform->GetRelativePos();
	Vec3 vRelativeScale		= pTransform->GetRelativeScale();
	Vec3 vRelativeRotation	= pTransform->GetRelativeRotation();
	 
	ImGui::Text("Trans");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Translation", vRelativePos))
	{
		pTransform->SetRelativePos(vRelativePos);
	}

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Scale", vRelativeScale))
	{
		pTransform->SetRelativeScale(vRelativeScale);
	}

	ImGui::Text("Roatation");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Rotation", vRelativeRotation))
	{
		pTransform->SetRelativeRotation(vRelativeRotation);
	}

	ImGui::Text("IndependentScale");
	ImGui::SameLine();
	bool Independent = pTransform->IsIndependentScale();
	if (ImGui::Checkbox("##IndependentScale", &Independent))
	{
		pTransform->SetIndependentScale(Independent);
	}
}