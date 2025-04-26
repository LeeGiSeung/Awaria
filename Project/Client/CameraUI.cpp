#include "pch.h"
#include "CameraUI.h"

#include <Engine/CCamera.h>

CameraUI::CameraUI()
	: ComponentUI(COMPONENT_TYPE::CAMERA)
{
}

CameraUI::~CameraUI()
{
}

void CameraUI::Update_UI()
{
}

void CameraUI::Render_UI()
{
	CGameObject* Target = GetTarget();
	CCamera* pCamera = Target->Camera();

	OutputTitle();

	int tab = 120;

	// 투영	
	ImGui::Text("Projection");
	ImGui::SameLine(tab);

	int type = pCamera->GetProjType();

	const char* items[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };	
	ImGui::Combo("##Projection", &type, items, 2);

	if (pCamera->GetProjType() != type)
	{
		pCamera->SetProjType((PROJ_TYPE)type);
	}

	// 투영 범위
	float Width			= pCamera->GetOrthoScaleX();
	float AspectRatio	= pCamera->GetAspectRatio();

	ImGui::BeginDisabled(type == PROJ_TYPE::PERSPECTIVE);
	{
		ImGui::Text("Width");
		ImGui::SameLine(tab);
		if (ImGui::InputFloat("##Width", &Width))
		{
			pCamera->SetOrthoScaleX(Width);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Aspect Ratio");
	ImGui::SameLine(tab);
	if (ImGui::InputFloat("##AspectRatio", &AspectRatio))
	{
		pCamera->SetAspectRatio(AspectRatio);
	}

	// 시야각(FieldOfView)
	float FOV = pCamera->GetFOV();
	ImGui::BeginDisabled(type == PROJ_TYPE::ORTHOGRAPHIC);
	{
		ImGui::Text("FOV");
		ImGui::SameLine(tab);
		if (ImGui::DragFloat("##FOV", &FOV, 2.f))
		{
			pCamera->SetFOV(FOV);
		}
	}
	ImGui::EndDisabled();
	
	// 최대 시야거리
	float Far = pCamera->GetFar();
	ImGui::Text("Far");
	ImGui::SameLine(tab);
	if (ImGui::InputFloat("##Far", &Far))
	{
		pCamera->SetFar(Far);
	}

	// 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~> : 서브 카메라
	int Priority = pCamera->GetPriority();
	ImGui::Text("Camera Priority");
	ImGui::SameLine(tab);
	if (ImGui::InputInt("##CameraPriority", &Priority))
	{
		pCamera->SetPriority(Priority);
	}
		 
	//UINT m_LayerCheck;   // 카메라가 렌더링할 레이어 비트설정
}
