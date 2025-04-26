#pragma once

class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;

	UINT			m_FPS;
	float			m_DT;	// Delta Time : 프레임 간격 시간, 1 프레임 동작하는데 걸리는 시간
	float			m_Time; // 프로그램을 킨 이후로 지난 누적시간

	float			m_EngineDT;
	float			m_EngineTime;

public:
	void Init();
	void Tick();

public:
	float GetDeltaTime() { return m_DT; }
	float GetEngineDeltaTime() { return m_EngineDT; }

	float GetTime() { return m_Time; }
	float GetEngineTime() { return m_EngineTime; }
};

