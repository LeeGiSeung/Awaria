#pragma once

class CGameObject;

class CTaskMgr
	: public singleton<CTaskMgr>
{
	SINGLE(CTaskMgr);
private:
	vector<tTask>			m_vecTask;
	vector<CGameObject*>	m_vecGC;




public:
	bool					m_LevelChanged;

	void Tick();
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }
	bool IsLevelChanged() { return m_LevelChanged; }
};

