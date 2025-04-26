#pragma once

class CEngine
	: public singleton<CEngine>
{
	SINGLE(CEngine);
private:
	HINSTANCE		m_hInst;		// ���μ��� �ּ�
	HWND			m_hMainWnd;		// ���� ������ �ڵ�
	Vec2			m_vResolution;	// ���� ������ �ػ�
	
	// FMOD ������ Ŭ����
	FMOD::System*	m_FMODSystem;	// FMOD �ֻ��� ������ ��ü


public:
	int Init(HINSTANCE _Inst, UINT _Width, UINT _Height, void(*_SAVE)(class CGameObject*, FILE*), CGameObject*(*_LOAD)(FILE*));
	void Progress();

	Vec2 GetResolution() { return m_vResolution; }
	HWND GetMainWndHwnd() { return m_hMainWnd; }

	FMOD::System* GetFMODSystem() { return m_FMODSystem ; }

private:
	int CreateMainWindow();
};

