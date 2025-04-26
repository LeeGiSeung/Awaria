#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI(COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Update_UI()
{
}

void Light2DUI::Render_UI()
{
	CGameObject* Target = GetTarget();
	CLight2D* pLight2D = Target->Light2D();

	OutputTitle();

	int tab = 100;

	// Light Type
	ImGui::Text("Light Type");
	ImGui::SameLine(tab);

	int type = (int)pLight2D->GetLightType();

	const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
	ImGui::Combo("##LightType", &type, items, 3);

	if (pLight2D->GetLightType() != (LIGHT_TYPE)type)
	{
		pLight2D->SetLightType((LIGHT_TYPE)type);
	}

	// Radius	
	ImGui::BeginDisabled(type == (int)LIGHT_TYPE::DIRECTIONAL);
	{
		float Radius = pLight2D->GetRadius();
		ImGui::Text("Radius");
		ImGui::SameLine(tab);
		if (ImGui::InputFloat("##Radius", &Radius))
		{
			pLight2D->SetRadius(Radius);
		}
	}
	ImGui::EndDisabled();

	
	// Angle
	ImGui::BeginDisabled(type != (int)LIGHT_TYPE::SPOT);
	{
		float Angle = pLight2D->GetAngle();
		ImGui::Text("Angle");
		ImGui::SameLine(tab);
		if (ImGui::InputFloat("##Angle", &Angle))
		{
			pLight2D->SetAngle(Angle);
		}
	}
	ImGui::EndDisabled();

	// LightColor
	Vec3 vColor = pLight2D->GetLightColor();
	ImGui::Text("Light Color");
	ImGui::SameLine(tab);
	 
	if (ImGui::ColorEdit3("##vColor", vColor))
	{
		pLight2D->SetLightColor(vColor);
	}	
}