#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
	, m_ShowColor(false)
	, m_ShowColorTime(0.f)
	, m_ChageShowState(true)
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Tick()
{
	for (UINT row = 0; row < MAX_LAYER; ++row)
	{
		for (UINT col = row; col < MAX_LAYER; ++col)
		{
			if (m_Matrix[row] & (1 << col))
			{
				CollisionLayer(row, col);
			}
		}
	}
}

void CCollisionMgr::CollisionLayerCheck(UINT _Left, UINT _Right)
{
	UINT Row = _Left;
	UINT Col = _Right;

	if (Col < Row)
	{
		Row = _Right;
		Col = _Left;
	}

	m_Matrix[Row] ^= (1 << Col);
}

void CCollisionMgr::CollisionLayer(UINT _Left, UINT _Right)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pLeftLayer = pCurLevel->GetLayer(_Left);
	CLayer* pRightLayer = pCurLevel->GetLayer(_Right);

	const vector<CGameObject*>& vecLeft = pLeftLayer->GetObjects();
	const vector<CGameObject*>& vecRight = pRightLayer->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (!vecLeft[i]->Collider2D())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			if (!vecRight[j]->Collider2D())
				continue;

			COLLISION_ID ID;
			ID.left  = vecLeft[i]->Collider2D()->GetID();
			ID.right = vecRight[j]->Collider2D()->GetID();
			map<DWORD_PTR, bool>::iterator iter = m_mapInfo.find(ID.ID);
			if (iter == m_mapInfo.end())
			{
				m_mapInfo.insert(make_pair(ID.ID, false));
				iter = m_mapInfo.find(ID.ID);
			}

			// 충돌 검사를 하는 두 오트젝트중 1개 이상이 삭제 예정상태라면
			bool IsDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			// 지금 충돌중이다
			if (IsOverlap(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// 이전에도 충돌했었다.
				if (iter->second)
				{
					if (IsDead)
					{
						vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
					}
					else
					{
						vecLeft[i]->Collider2D()->Overlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->Overlap(vecLeft[i]->Collider2D());
					}
				}

				// 이전에는 충돌하지 않고 있었다
				else
				{
					if (!IsDead)
					{
						vecLeft[i]->Collider2D()->BeginOverlap(vecRight[j]->Collider2D());
						vecRight[j]->Collider2D()->BeginOverlap(vecLeft[i]->Collider2D());
					}
				}			

				iter->second = true;
			}

			// 지금 충돌중이 아니다.
			else
			{
				// 이전에는 충돌하고 있었다.
				if (iter->second)
				{
					vecLeft[i]->Collider2D()->EndOverlap(vecRight[j]->Collider2D());
					vecRight[j]->Collider2D()->EndOverlap(vecLeft[i]->Collider2D());
				}

				iter->second = false;
			}
		}
	}
}

bool CCollisionMgr::IsOverlap(CCollider2D* _LeftCol, CCollider2D* _RightCol)
{
	const Matrix& matLeft = _LeftCol->GetWorldMat();
	const Matrix& matRight = _RightCol->GetWorldMat();

	// 행렬 4행의 위치값(충돌체의 월드 위치)
	Vec3 vLeftPos = matLeft.Translation();
	Vec3 vRightPos = matRight.Translation();

	// 두 충돌체의 중심을 잇는 벡터
	Vec3 vCenter = vRightPos - vLeftPos;

	// 월드상의 충돌체의 꼭지점 위치를 찾아서, 각 표면 방향을 알아낸다.
	// 이 방향벡터를 투영축으로 사용할 것
	Vec3 vProj[4] = {};
	vProj[0] = XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matLeft);
	vProj[1] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matLeft);	
	vProj[2] = XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.f), matRight) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matRight);
	vProj[3] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), matRight) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matRight);

	for (int j = 0; j < 4; ++j)
	{
		// 4개의 투영축 중에서 하나를 투영시킬 목적지 방향으로 정한다.
		Vec3 vProjTarget = vProj[j];
		vProjTarget.Normalize();

		// 해당 투영 축으로 나머지 4개의 투영축 벡터를 투영시킨 거리의 절반을 구한다.
		float ProjDist = 0.f;
		for (int i = 0; i < 4; ++i)
		{
			ProjDist += fabs(vProjTarget.Dot(vProj[i]));
		}
		ProjDist /= 2.f;

		// 두 충돌체의 중심점을 이은 벡터도 투영시킨다.
		float fCenter = fabs(vProjTarget.Dot(vCenter));

		// 중심끼리 이은 벡터의 투영길이가, 두 충돌체의 투영 면적 절반보다 멀면, 
		// 둘 사이를 나눌 수 있는 분리축이 존재한다.
		if (ProjDist < fCenter)
			return false;
	}

	// 모든 테스트를 다 통과하면, 투 충돌체는 겹쳐있다.
	return true;
}
