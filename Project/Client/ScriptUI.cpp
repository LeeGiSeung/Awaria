#include "pch.h"
#include "ScriptUI.h"

#include <Lecture/CScriptMgr.h>
#include <Engine/CScript.h>
#include <Engine/assets.h>

UINT ScriptUI::ScriptUINextID = 0;

ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_TargetScript(nullptr)
	, m_ScriptUI_ID(ScriptUINextID++)
{
	// ScriptUI ���� �̸��� �ٸ��� �����ǵ��� ��
	char szBuff[10] = {};
	_itoa_s(m_ScriptUI_ID, szBuff, 10);

	// ���� �̸��� �ڿ� �Ȱ�ġ�� ���ڸ� �ٿ���
	SetName(GetName() + szBuff);	 
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	if (nullptr == m_TargetScript)
	{
		SetActive(false);
	}
	else
	{
		SetActive(true);
	}
}

void ScriptUI::Render_UI()
{
	string ScriptName = ToString(CScriptMgr::GetScriptName(m_TargetScript));	
	OutputTitle(ScriptName);	

	const vector<tScriptParam>& ScriptParam = m_TargetScript->GetScriptParam();
	for (size_t i = 0; i < ScriptParam.size(); ++i)
	{	
		char szID[50] = {};
		sprintf_s(szID, 50, "##WidgetID_%d", (int)i);

		switch (ScriptParam[i].Type)
		{
		case SCRIPT_PARAM::INT:
		{			
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);			
			ImGui::InputInt(szID, (int*)ScriptParam[i].pData);			
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::FLOAT:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);
			ImGui::InputFloat(szID, (float*)ScriptParam[i].pData);
			AddItemHeight();
		}		
			break;
		case SCRIPT_PARAM::VEC2:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);
			ImGui::InputFloat2(szID, *((Vec2*)ScriptParam[i].pData));
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC3:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);
			ImGui::InputFloat3(szID, *((Vec3*)ScriptParam[i].pData));
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::VEC4:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);
			ImGui::InputFloat4(szID, *((Vec4*)ScriptParam[i].pData));
			AddItemHeight();
		}
			break;
		case SCRIPT_PARAM::TEXTURE:

			break;
		case SCRIPT_PARAM::PREFAB:
		{
			ImGui::Text(ScriptParam[i].Desc.c_str());
			ImGui::SameLine(100);
			Ptr<CPrefab> pPrefab = *((Ptr<CPrefab>*)ScriptParam[i].pData);

			string AssetName;
			if (nullptr != pPrefab)
				AssetName = ToString(pPrefab->GetKey());

			ImGui::InputText(szID, (char*)AssetName.c_str(), AssetName.length() + 1, ImGuiInputTextFlags_ReadOnly);

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
					if (ASSET_TYPE::PREFAB == pAsset->GetType())
					{
						pPrefab = dynamic_cast<CPrefab*>(pAsset);
						assert(pPrefab.Get());						
					}
				}

				ImGui::EndDragDropTarget();
			}

			// �������� �־��ش�.
			*((Ptr<CPrefab>*)ScriptParam[i].pData) = pPrefab;

			AddItemHeight();
		}
			break;
		}
	}

	//	
}



