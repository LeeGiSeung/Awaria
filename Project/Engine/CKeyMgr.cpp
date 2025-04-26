#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"

UINT KeyValue[KEY::KEY_END] =
{
	'Q', 'W', 'E', 'R', 'T', 'Y',
	'A', 'S', 'D', 'F', 'G', 'H',
	'Z', 'X', 'C',  'V',  'B',  'N', 'M',

	VK_RETURN,
	VK_ESCAPE,
	VK_LSHIFT,
	VK_MENU,
	VK_CONTROL,
	VK_SPACE,

	VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,	

	VK_LBUTTON,
	VK_RBUTTON,
};




CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}

void CKeyMgr::Init()
{
	for (size_t i = 0; i < KEY_END; ++i)
	{
		m_vecKeyInfo.push_back(KeyInfo{ KEY_STATE::NONE, false});
	}	

	// 초기 마우스좌표 세팅
	POINT ptPos = {};
	GetCursorPos(&ptPos);
	ScreenToClient(CEngine::GetInst()->GetMainWndHwnd(), &ptPos);
	
	m_MousePos = Vec2((float)ptPos.x, (float)ptPos.y);	
	m_MousePrevPos = m_MousePos;	
}

void CKeyMgr::Tick()
{
	// 게임 윈도우가 포커싱 중일 때
	if (GetFocus() == CEngine::GetInst()->GetMainWndHwnd())
	{
		for (size_t i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			// 현재 해당 키가 눌려있다.
			if (GetAsyncKeyState(KeyValue[i]) & 0x8001)
			{
				// 이전에도 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = KEY_STATE::PRESSED;
				}

				// 이전에 눌려있지 않았다.
				else
				{
					m_vecKeyInfo[i].State = KEY_STATE::TAP;
				}

				m_vecKeyInfo[i].bPrevPressed = true;
			}

			// 현재 해당키가 안눌려있다.
			else
			{
				// 이전에는 눌려있었다.
				if (m_vecKeyInfo[i].bPrevPressed)
				{
					m_vecKeyInfo[i].State = KEY_STATE::RELEASED;
				}

				else
				{
					m_vecKeyInfo[i].State = KEY_STATE::NONE;
				}

				m_vecKeyInfo[i].bPrevPressed = false;
			}
		}

		// 마우스 좌표 갱신
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CEngine::GetInst()->GetMainWndHwnd(), &ptPos);

		// 이전 프레임 마우스좌표 기록
		m_MousePrevPos = m_MousePos;

		// 현재 마우스 좌표 기록
		m_MousePos = Vec2((float)ptPos.x, (float)ptPos.y);

		// 마우스 커서가 진행하는 방향벡터를 구함
		m_DragDir = m_MousePos - m_MousePrevPos;
	}

	// 게임 윈도우가 비활성화 되어 있을 때
	else
	{
		for (size_t i = 0; i < m_vecKeyInfo.size(); ++i)
		{
			if (KEY_STATE::TAP == m_vecKeyInfo[i].State
				|| KEY_STATE::PRESSED == m_vecKeyInfo[i].State)
			{
				m_vecKeyInfo[i].State = KEY_STATE::RELEASED;
			}

			else if (KEY_STATE::RELEASED == m_vecKeyInfo[i].State)
			{
				m_vecKeyInfo[i].State = KEY_STATE::NONE;
			}

			m_vecKeyInfo[i].bPrevPressed = false;
		}

		// 마우스 좌표 갱신
		*((int*)&m_MousePos.x) = 0xffffffff;
		*((int*)&m_MousePos.y) = 0xffffffff;
	}
}

bool CKeyMgr::IsMouseOffScreen()
{
	Vec2 vResolution = CEngine::GetInst()->GetResolution();

	if (vResolution.x <= m_MousePos.x || vResolution.y <= m_MousePos.y
		|| m_MousePos.x < 0 || m_MousePos.y < 0)
	{
		return true;
	}
	else
	{
		return false;
	}	
}