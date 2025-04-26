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
	
	// 재질 이름
	Material();

	// 재질이 참조하는 쉐이더 정보
	Shader();	
	
	// 재질을 통해서 쉐이더에 전달할 상수 데이터
	ScalarParameter();

	// 텍스쳐 데이터
	TextureParameter();
}

void MaterialUI::Material()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// 재질 이름
	ImGui::Text("Name");
	ImGui::SameLine(100);

	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::SetNextItemWidth(200);
	ImGui::InputText("##MtrlKey", (char*)MtrlKey.c_str(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// 그래픽 쉐이더 정보
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
		// ContentUI 에서 드래그된 데이터만 받는다.
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (nullptr != payload)
		{
			// 전달된 데이터에 들어있는 Asset 의 주소값을 꺼내온다.
			CAsset* pAsset = *((CAsset**)payload->Data);

			// 꺼내온 에셋이 Mesh 타입인 경우에만 작업을 이어간다.
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetType())
			{
				Ptr<CGraphicShader> pShader = dynamic_cast<CGraphicShader*>(pAsset);
				assert(pShader.Get());
				pMtrl->SetShader(pShader);
			}
		}

		ImGui::EndDragDropTarget();
	}
	 
	// 쉐이더 선택 버튼
	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18, 18)))
	{
		// ListUI 를 활성화 한다.
		ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
		assert(pListUI);

		// ListUI 초기화 및 활성화
		pListUI->SetActive(true);

		// ListUI 에서 보여지길 원하는 문자열 항목 추가
		vector<string> vecShaderName;
		CAssetMgr::GetInst()->GetAssetName(GRAPHIC_SHADER, vecShaderName);

		// 리스트에 출력할 항목정보 추가
		pListUI->AddItem(vecShaderName);

		// 항목이 DoubleClicked 됐을 때 실행할 함수 전달
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

		// 재질이 전달하는 텍스쳐를 가져옴
		Ptr<CTexture> pTex = pMtrl->GetTexParam(TexParam[i].Type);
		
		// 텍스쳐 	
		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
		ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

		ImTextureID texid = 0;
		if (nullptr != pTex)
			texid = (ImTextureID)pTex->GetSRV().Get();		
				
		ImGui::Image(texid			// 렌더링할 텍스쳐 ShaderResourceView
			, ImVec2(150.f, 150.f)	// 텍스쳐 위젯 크기
			, uv_min, uv_max		// 출력할 텍스쳐 UV 범위
			, tint_col, border_col);// 틴트, 보더 컬러

		// 쉐이더 선택 버튼
		ImGui::SameLine();
		if (ImGui::Button("##TexBtn", ImVec2(18, 18)))
		{
			m_TargetTex = TexParam[i].Type;

			// ListUI 를 활성화 한다.
			ListUI* pListUI = ImGuiMgr::GetInst()->FindUI<ListUI>("ListUI");
			assert(pListUI);

			// ListUI 초기화 및 활성화
			pListUI->SetActive(true);

			// ListUI 에서 보여지길 원하는 문자열 항목 추가
			vector<string> TexName;
			CAssetMgr::GetInst()->GetAssetName(TEXTURE, TexName);

			// 리스트에 출력할 항목정보 추가
			pListUI->AddItem(TexName);

			// 항목이 DoubleClicked 됐을 때 실행할 함수 전달
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

	// 변경점을 저장파일에 반영하기
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
