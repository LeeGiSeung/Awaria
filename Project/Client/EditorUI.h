#pragma once

#include "imgui/imgui.h"
#include "ImGuiMgr.h"

class EditorUI
{
private:
	string				m_Name;
	bool				m_Active;

	EditorUI*			m_ParentUI;
	vector<EditorUI*>	m_vecChildUI;

	ImVec2				m_ChildSize;

	bool				m_Modal;
	bool				m_Seperator;
	bool				m_Focused;	// 자신이 포커싱 되어있는 상태인지 확인

	 
public:
	void SetName(const string& _Name) { m_Name = _Name; }
	const string& GetName() { return m_Name; }

	bool IsActive() { return m_Active; }
	void SetActive(bool _Active);

	void SetModal(bool _Modal) { m_Modal = _Modal; }

	virtual void Activate() {};
	virtual void Deactivate() {};

	void AddChildUI(EditorUI* _ChildUI)
	{
		m_vecChildUI.push_back(_ChildUI);
		_ChildUI->m_ParentUI = this;
	}

	void SetSizeAsChild(ImVec2 _Size) { m_ChildSize = _Size; }
	ImVec2 GetSizeAsChild() { return m_ChildSize; }
	
	void SetSeperator(bool _Seperator) { m_Seperator = _Seperator; }

public:
	virtual void Init() {};   // UI 초기화
	virtual void Update(); // 매 프레임마다 UI 가 할 일
	virtual void Render(); // UI 렌더링
	
	virtual void Update_UI() = 0;
	virtual void Render_UI() = 0;

	virtual void OnFocus() {};
	virtual void OffFocus() {};

private:
	void CheckFocus();
	

public:
	EditorUI();
	virtual ~EditorUI();
};

