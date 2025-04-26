#pragma once

class CGameObjectEX;

class CEditorMgr
	: public singleton<CEditorMgr>
{
	SINGLE(CEditorMgr)
private:
	vector<CGameObjectEX*>	m_vecEditorObj;

public:
	void Init();
	void Progress();
};

