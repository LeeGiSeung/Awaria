#include "pch.h"
#include "MaterialUI.h"

#include "ImGuiMgr.h"
#include "ListUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CGraphicShader.h>


MaterialUI::MaterialUI()
	: AssetUI(MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}


void MaterialUI::Update_UI()
{
}

void MaterialUI::Render_UI()
{
	OutputTitle();
	
	// ���� �̸�
	Material();

	// ������ �����ϴ� ���̴� ����
	Shader();	
	
	// ������ ���ؼ� ���̴��� ������ ��� ������
	ScalarParameter();

	// �ؽ��� ������
	TextureParameter();
}

void MaterialUI::Material()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// ���� �̸�
	ImGui::Text("Name");
	ImGui::SameLine(100);

	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MtrlKey", (char*)MtrlKey.c_str(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// �׷��� ���̴� ����
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();
	ImGui::Text("GraphicShader");
	ImGui::SameLine(100);
}

void MaterialUI::Shader()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	string ShaderKey;
	if (nullptr != pShader)
	{
		ShaderKey = string(pShader->GetKey().begin(), pShader->GetKey().end());
	}
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##ShaderKey", (char*)ShaderKey.c_str(), ShaderKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

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
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetType())
			{
				Ptr<CGraphicShader> pShader = dynamic_cast<CGraphicShader*>(pAsset);
				assert(pShader.Get());
				pMtrl->SetShader(pShader);
			}
		}

		ImGui::EndDragDropTarget();
	}
	 
	// ���̴� ���� ��ư
	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18, 18)))
	{
		// ListUI �� Ȱ��ȭ �Ѵ�.
		ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
		assert(pListUI);

		// ListUI �ʱ�ȭ �� Ȱ��ȭ
		pListUI->SetActive(true);

		// ListUI ���� �������� ���ϴ� ���ڿ� �׸� �߰�
		vector<string> vecShaderName;
		CAssetMgr::GetInst()->GetAssetName(GRAPHIC_SHADER, vecShaderName);

		// ����Ʈ�� ����� �׸����� �߰�
		pListUI->AddItem(vecShaderName);

		// �׸��� DoubleClicked ���� �� ������ �Լ� ����
		pListUI->AddDelegate(this, (EUI_DELEGATE_1)&MaterialUI::SelectShader);
	}
}

void MaterialUI::ScalarParameter()
{
	ImGui::NewLine();
	ImGui::NewLine();

	ImGui::Text("Material Parameter");
	ImGui::NewLine();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	const vector<tScalarParam>& ScalarParam = pShader->GetScalarParam();	

	for (size_t i = 0; i < ScalarParam.size(); ++i)
	{
		char szID[50] = {};
		sprintf_s(szID, 50, "##WidgetID_%d", (int)i);

		switch (ScalarParam[i].Type)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			string Desc = ToString(ScalarParam[i].Desc);
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(100);

			int Data = pMtrl->GetScalarParam<int>(ScalarParam[i].Type);
			if (ImGui::InputInt(szID, &Data))
			{
				pMtrl->SetScalarParam(ScalarParam[i].Type, Data);
			}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			string Desc = ToString(ScalarParam[i].Desc);
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(100);

			float Data = pMtrl->GetScalarParam<float>(ScalarParam[i].Type);
			if (ImGui::InputFloat(szID, &Data))
			{
				pMtrl->SetScalarParam(ScalarParam[i].Type, Data);
			}
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			string Desc = ToString(ScalarParam[i].Desc);
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(100);

			Vec2 Data = pMtrl->GetScalarParam<Vec2>(ScalarParam[i].Type);
			if (ImGui::InputFloat2(szID, Data))
			{
				pMtrl->SetScalarParam(ScalarParam[i].Type, Data);
			}
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			string Desc = ToString(ScalarParam[i].Desc);
			ImGui::Text(Desc.c_str());
			ImGui::SameLine(100);

			Vec4 Data = pMtrl->GetScalarParam<Vec4>(ScalarParam[i].Type);
			if (ImGui::InputFloat4(szID, Data))
			{
				pMtrl->SetScalarParam(ScalarParam[i].Type, Data);
			}
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			string Desc = ToString(ScalarParam[i].Desc);
			ImGui::Text(Desc.c_str());

			Matrix Data = pMtrl->GetScalarParam<Matrix>(ScalarParam[i].Type);

			bool bChanged = false;

			string strID = szID;

			strID += '0';
			if (ImGui::InputFloat4(strID.c_str(), &Data._11))
				bChanged = true;

			strID += '1';
			if (ImGui::InputFloat4("##InputMat1", &Data._21))
				bChanged = true;

			strID += '2';
			if (ImGui::InputFloat4("##InputMat2", &Data._31))
				bChanged = true;

			strID += '3';
			if (ImGui::InputFloat4("##InputMat3", &Data._41))
				bChanged = true;

			if (bChanged)
			{
				pMtrl->SetScalarParam(ScalarParam[i].Type, Data);
			}
		}
		break;
		}
	}
}

void MaterialUI::TextureParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	const vector<tTexParam>& TexParam = pShader->GetTexParam();

	for (size_t i = 0; i < TexParam.size(); ++i)
	{
		ImGui::Text(ToString(TexParam[i].Desc).c_str());		

		// ������ �����ϴ� �ؽ��ĸ� ������
		Ptr<CTexture> pTex = pMtrl->GetTexParam(TexParam[i].Type);
		
		// �ؽ��� 	
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
		ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

		ImTextureID texid = 0;
		if (nullptr != pTex)
			texid = (ImTextureID)pTex->GetSRV().Get();		
				
		ImGui::Image(texid			// �������� �ؽ��� ShaderResourceView
			, ImVec2(150.f, 150.f)	// �ؽ��� ���� ũ��
			, uv_min, uv_max		// ����� �ؽ��� UV ����
			, tint_col, border_col);// ƾƮ, ���� �÷�

		// ���̴� ���� ��ư
		ImGui::SameLine();
		if (ImGui::Button("##TexBtn", ImVec2(18, 18)))
		{
			m_TargetTex = TexParam[i].Type;

			// ListUI �� Ȱ��ȭ �Ѵ�.
			ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
			assert(pListUI);

			// ListUI �ʱ�ȭ �� Ȱ��ȭ
			pListUI->SetActive(true);

			// ListUI ���� �������� ���ϴ� ���ڿ� �׸� �߰�
			vector<string> TexName;
			CAssetMgr::GetInst()->GetAssetName(TEXTURE, TexName);

			// ����Ʈ�� ����� �׸����� �߰�
			pListUI->AddItem(TexName);

			// �׸��� DoubleClicked ���� �� ������ �Լ� ����
			pListUI->AddDelegate(this, (EUI_DELEGATE_1)&MaterialUI::SelectTexture);
		}
	}
}

void MaterialUI::SelectShader(DWORD_PTR _Data)
{
	ListUI* pListUI = (ListUI*)_Data;
	string strSelected = pListUI->GetSelected();

	wstring Selected = wstring(strSelected.begin(), strSelected.end());

	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(Selected);
	assert(pShader.Get());

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	pMtrl->SetShader(pShader);

	// �������� �������Ͽ� �ݿ��ϱ�
	wstring FilePath = CPathMgr::GetContentPath();	
	pMtrl->Save(FilePath + pMtrl->GetRelativePath());
}

void MaterialUI::SelectTexture(DWORD_PTR _Data)
{
	ListUI* pListUI = (ListUI*)_Data;
	string strSelected = pListUI->GetSelected();
	wstring Selected = wstring(strSelected.begin(), strSelected.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(Selected);
	assert(pTex.Get());

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	pMtrl->SetTexParam(m_TargetTex, pTex);
}
