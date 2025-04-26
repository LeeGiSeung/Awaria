#pragma once


union COLLISION_ID
{
	struct
	{
		UINT left;
		UINT right;
	};

	DWORD_PTR ID;
};


class CCollider2D;

class CCollisionMgr
	: public singleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT				 m_Matrix[MAX_LAYER];
	map<DWORD_PTR, bool> m_mapInfo;
	bool				 m_ShowColor;
	bool				 m_ChageShowState;
	float                m_ShowColorTime;


public:
	bool GetShowCollider() { return m_ShowColor; }
	void SetShowCollider(bool _in) { m_ShowColor = _in; }

	bool GetCanChangeShowColor() { return m_ChageShowState; }
	void SetCanChangeShowColor(bool _in) { m_ChageShowState = _in; }

	float GetShowColorTime() { return m_ShowColorTime; }
	void  SetShowColorTime(float _in) {
		if (_in == 0) {
			m_ShowColorTime = 0.f;
		}
		else {
			m_ShowColorTime += _in;
		}
	}

	void Tick();
	void CollisionLayerCheck(UINT _Left, UINT _Right);

private:
	void CollisionLayer(UINT _Left, UINT _Right);
	bool IsOverlap(CCollider2D* _LeftCol, CCollider2D* _RightCol);

};

