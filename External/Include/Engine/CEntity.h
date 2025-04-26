#pragma once

class CEntity
{
private:
	static UINT		g_NextId;
	UINT			m_ID;
	wstring			m_Name;

public:
	void SetName(wstring _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }

	UINT GetID() { return m_ID; }
	void SetID(int _in) { m_ID = _in; }

	// 자기자신의 복제객체를 반환해주는 함수
	virtual CEntity* Clone() = 0;
public:
	CEntity();
	CEntity(const CEntity& _Origin);
	virtual ~CEntity();
};

