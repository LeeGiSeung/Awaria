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

	// ����	
	ImGui::Text("Projection");
	ImGui::SameLine(tab);

	int type = pCamera->GetProjType();

	const char* items[] = { "ORTHOGRAPHIC", "PERSPECTIVE" };	
	ImGui::Combo("##Projection", &type, items, 2);

	if (pCamera->GetProjType() != type)
	{
		pCamera->SetProjType((PROJ_TYPE)type);
	}

	// ���� ����
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

	// �þ߰�(FieldOfView)
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
	
	// �ִ� �þ߰Ÿ�
	float Far = pCamera->GetFar();
	ImGui::Text("Far");
	ImGui::SameLine(tab);
	if (ImGui::InputFloat("##Far", &Far))
	{
		pCamera->SetFar(Far);
	}

	// ī�޶� �켱����, -1 : �̵�� ī�޶�, 0 : ���� ī�޶�, 1 ~> : ���� ī�޶�
	int Priority = pCamera->GetPriority();
	ImGui::Text("Camera Priority");
	ImGui::SameLine(tab);
	if (ImGui::InputInt("##CameraPriority", &Priority))
	{
		pCamera->SetPriority(Priority);
	}
		 
	//UINT m_LayerCheck;   // ī�޶� �������� ���̾� ��Ʈ����
}
