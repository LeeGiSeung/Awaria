#pragma once


class CLevelMgr
	: public singleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	class CLevel*	m_CurLevel;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	LEVEL_STATE GetLevelState();

private:
	void ChangeLevel(CLevel* _NextLevel);


public:
	void Init();
	void Progress();

	friend class CTaskMgr;
};

