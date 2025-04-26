#pragma once

class EditorUI;


typedef void(EditorUI::* EUI_DELEGATE_0)(void);
typedef void(EditorUI::* EUI_DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* EUI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);


class ImGuiMgr
	: public singleton<ImGuiMgr>
{
	SINGLE(ImGuiMgr);
private:
	map<string, EditorUI*>	m_mapUI;
	bool					m_ShowDemo;
	 
public:
	template<typename T>
	T* FindUI(const string& _Name)
	{
		map<string, EditorUI*>::iterator iter = m_mapUI.find(_Name);
		if (iter == m_mapUI.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}

public:
	void Init(HWND _hMainWind, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context);
	void Progress();

private:
	void CreateEditorUI();

};

