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

			// �浹 �˻縦 �ϴ� �� ��Ʈ��Ʈ�� 1�� �̻��� ���� �������¶��
			bool IsDead = vecLeft[i]->IsDead() || vecRight[j]->IsDead();

			// ���� �浹���̴�
			if (IsOverlap(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D()))
			{
				// �������� �浹�߾���.
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

				// �������� �浹���� �ʰ� �־���
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

			// ���� �浹���� �ƴϴ�.
			else
			{
				// �������� �浹�ϰ� �־���.
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

	// ��� 4���� ��ġ��(�浹ü�� ���� ��ġ)
	Vec3 vLeftPos = matLeft.Translation();
	Vec3 vRightPos = matRight.Translation();

	// �� �浹ü�� �߽��� �մ� ����
	Vec3 vCenter = vRightPos - vLeftPos;

	// ������� �浹ü�� ������ ��ġ�� ã�Ƽ�, �� ǥ�� ������ �˾Ƴ���.
	// �� ���⺤�͸� ���������� ����� ��
	Vec3 vProj[4] = {};
	vProj[0] = XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matLeft);
	vProj[1] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), matLeft) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matLeft);	
	vProj[2] = XMVector3TransformCoord(Vec3(0.5f, 0.5f, 0.f), matRight) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matRight);
	vProj[3] = XMVector3TransformCoord(Vec3(-0.5f, -0.5f, 0.f), matRight) - XMVector3TransformCoord(Vec3(-0.5f, 0.5f, 0.f), matRight);

	for (int j = 0; j < 4; ++j)
	{
		// 4���� ������ �߿��� �ϳ��� ������ų ������ �������� ���Ѵ�.
		Vec3 vProjTarget = vProj[j];
		vProjTarget.Normalize();

		// �ش� ���� ������ ������ 4���� ������ ���͸� ������Ų �Ÿ��� ������ ���Ѵ�.
		float ProjDist = 0.f;
		for (int i = 0; i < 4; ++i)
		{
			ProjDist += fabs(vProjTarget.Dot(vProj[i]));
		}
		ProjDist /= 2.f;

		// �� �浹ü�� �߽����� ���� ���͵� ������Ų��.
		float fCenter = fabs(vProjTarget.Dot(vCenter));

		// �߽ɳ��� ���� ������ �������̰�, �� �浹ü�� ���� ���� ���ݺ��� �ָ�, 
		// �� ���̸� ���� �� �ִ� �и����� �����Ѵ�.
		if (ProjDist < fCenter)
			return false;
	}

	// ��� �׽�Ʈ�� �� ����ϸ�, �� �浹ü�� �����ִ�.
	return true;
}
