#include "pch.h"
#include "RenderComUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CRenderComponent.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

RenderComUI::RenderComUI(COMPONENT_TYPE _Type)
	: ComponentUI(_Type)
{
}

RenderComUI::~RenderComUI()
{
}

void RenderComUI::Render_UI()
{
	CGameObject* pObject = GetTarget();
	CRenderComponent* pRenderCom = pObject->GetRenderComponent();

	OutputTitle();

	int tab = 100;

	Ptr<CMesh> pMesh = pRenderCom->GetMesh();
	Ptr<CMaterial> pMtrl = pRenderCom->GetMaterial();
	 
	// Mesh	
	string MeshKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	ImGui::Text("Mesh");
	ImGui::SameLine(tab);		
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MeshName", (char*)MeshKey.c_str(), MeshKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI ���� �巡�׵� �����͸� �޴´�.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// ���޵� �����Ϳ� ����ִ� Asset �� �ּҰ��� �����´�.
			CAsset* pAsset = *((CAsset**)payload->Data);

			// ������ ������ Mesh Ÿ���� ��쿡�� �۾��� �̾��.
			if (ASSET_TYPE::MESH == pAsset->GetType())
			{
				CMesh* pMesh = dynamic_cast<CMesh*>(pAsset);
				assert(pMesh);
				pRenderCom->SetMesh(pMesh);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18, 18)))
	{
		// ListUI �� Ȱ��ȭ �Ѵ�.
		ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
		assert(pListUI);

		// ListUI �ʱ�ȭ �� Ȱ��ȭ
		pListUI->SetActive(true);

		// ListUI ���� �������� ���ϴ� ���ڿ� �׸� �߰�
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetName(MESH, vecMeshNames);
		pListUI->AddItem(vecMeshNames);

		// �׸��� DoubleClicked ���� �� ������ �Լ� ����
		pListUI->AddDelegate(this, (EUI_DELEGATE_1)&RenderComUI::SelectMesh);
	}

	// Material
	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::Text("Material");
	ImGui::SameLine(tab);
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MtrlName", (char*)MtrlKey.c_str(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Drop Check
	if (ImGui::BeginDragDropTarget())
	{
		// ContentUI ���� �巡�׵� �����͸� �޴´�.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// ���޵� �����Ϳ� ����ִ� Asset �� �ּҰ��� �����´�.
			CAsset* pAsset = *((CAsset**)payload->Data);

			// ������ ������ Mesh Ÿ���� ��쿡�� �۾��� �̾��.
			if (ASSET_TYPE::MATERIAL == pAsset->GetType())
			{
				CMaterial* pMtrl = dynamic_cast<CMaterial*>(pAsset);
				assert(pMtrl);
				pRenderCom->SetMaterial(pMtrl);
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(18, 18)))
	{
		// ListUI �� Ȱ��ȭ �Ѵ�.
		ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
		assert(pListUI);

		pListUI->SetActive(true);

		vector<string> vecMtrlName;
		CAssetMgr::GetInst()->GetAssetName(MATERIAL, vecMtrlName);
		pListUI->AddItem(vecMtrlName);

		pListUI->AddDelegate(this, (EUI_DELEGATE_1)&RenderComUI::SelectMaterial);
	}
}

void RenderComUI::SelectMesh(DWORD_PTR _Data)
{
	ListUI* pListUI = (ListUI*)_Data;
	string strSelected = pListUI->GetSelected();

	wstring Selected = wstring(strSelected.begin(), strSelected.end());

	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(Selected);
	assert(pMesh.Get());

	CGameObject* pObject = GetTarget();
	CRenderComponent* pRenderCom = pObject->GetRenderComponent();
	pRenderCom->SetMesh(pMesh);
}

void RenderComUI::SelectMaterial(DWORD_PTR _Data)
{
	ListUI* pListUI = (ListUI*)_Data;
	string strSelected = pListUI->GetSelected();

	wstring Selected = wstring(strSelected.begin(), strSelected.end());

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(Selected);
	assert(pMtrl.Get());

	CGameObject* pObject = GetTarget();
	CRenderComponent* pRenderCom = pObject->GetRenderComponent();
	pRenderCom->SetMaterial(pMtrl);
}
