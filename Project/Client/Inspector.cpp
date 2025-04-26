#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "ScriptUI.h"
#include "AssetUI.h"

Inspector::Inspector()
	: m_Object(nullptr)
	, m_ComUI{}
{
}

Inspector::~Inspector()
{
}

void Inspector::Update_UI()
{
	
}

void Inspector::Render_UI()
{
	if (nullptr == m_Object)
		return;

	string name(m_Object->GetName().begin(), m_Object->GetName().end());
	ImGui::Text(name.c_str());

	ImGui::Separator();
}


void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_Object = _Object;

	// Inspector �� �ڽ�UI(ComponentUI) �鿡�Ե� Object �� �˷��ش�.
	for (UINT i = 0; i < COMPONENT_END; ++i)
	{
		if (nullptr == m_ComUI[i])
			continue;

		m_ComUI[i]->SetTarget(m_Object);
	}	

	// Inspector �� ������ ScriptUI ������ Object �� ������ Script �������� ������, �߰��� �� �����Ѵ�.
	if (nullptr != m_Object)
	{
		size_t ScriptCount = m_Object->GetScripts().size();
		if (m_vecScriptUI.size() < ScriptCount)
		{
			int SpawnCount = ScriptCount - m_vecScriptUI.size();
			for (int i = 0; i < SpawnCount; ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				AddChildUI(pScriptUI);
				m_vecScriptUI.push_back(pScriptUI);
			}
		}
	}	

	// Inspector �� ��� ScriptUI ���� Object ������ �˷��ش�.
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (nullptr == m_Object || (m_Object->GetScripts().size() <= i))
			m_vecScriptUI[i]->SetScript(nullptr);
		else
		{
			m_vecScriptUI[i]->SetScript(_Object->GetScripts()[i]);
		}
	}

	// ��� ���� UI�� ���� ��Ȱ��ȭ ��Ų��.
	if (nullptr != _Object)
	{		
		for (UINT i = 0; i < ASSET_END; ++i)
		{
			if (nullptr != m_AssetUI[i])
				m_AssetUI[i]->SetAsset(nullptr);
		}
	}
}

void Inspector::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = _Asset;
		
	for (UINT i = 0; i < ASSET_END; ++i)
	{
		if(nullptr != m_AssetUI[i])
			m_AssetUI[i]->SetAsset(m_Asset);
	}

	SetTargetObject(nullptr);
}
