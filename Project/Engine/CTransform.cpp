#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"


CTransform::CTransform()
	: CComponent(TRANSFORM)
	, m_RelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_IndependentScale(false)
{
	m_RelativeDir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_RelativeDir[(UINT)DIR::UP]	= Vec3(0.f, 1.f, 0.f);
	m_RelativeDir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}

void CTransform::FinalTick()
{
	// ��ġ, ũ��, ȸ�� ������ ������ķ� ��ȯ
	// Scale 
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);

	// Rotation
	Matrix matRotation = XMMatrixRotationX(m_RelativeRot.x);
	matRotation *= XMMatrixRotationY(m_RelativeRot.y);
	matRotation *= XMMatrixRotationZ(m_RelativeRot.z);

	// Translation
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// ũ�� ȸ�� �̵� �θ� ������ ����
	m_matWorld = matScale * matRotation * matTrans;

	// ������Ʈ�� �������� ���
	m_RelativeDir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_RelativeDir[(UINT)DIR::UP]	= Vec3(0.f, 1.f, 0.f);
	m_RelativeDir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);

	// ���⺤�Ϳ� ȸ������� �����ؼ� ���� ���Ⱚ�� ����Ѵ�.
	// XMVector3TransformCoord()  w ������ǥ ���� 1�� Ȯ��
	// XMVector3TransformNormal() w ������ǥ���� 0���� Ȯ��
	for (int i = 0; i < (int)DIR::END; ++i)
	{
		m_WorldDir[i] = m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRotation);
	}

	// �θ� ������Ʈ�� �ִ� ���, �θ��� ��������� ������Ų��.
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_IndependentScale)
		{
			Vec3 vParentScale = pParent->Transform()->GetWorldScale();
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z));
			m_matWorld = m_matWorld * matParentScaleInv * pParent->Transform()->GetWorldMat();
		}
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}

		m_WorldDir[(UINT)DIR::RIGHT]	= Vec3(1.f, 0.f, 0.f);
		m_WorldDir[(UINT)DIR::UP]		= Vec3(0.f, 1.f, 0.f);
		m_WorldDir[(UINT)DIR::FRONT]	= Vec3(0.f, 0.f, 1.f);

		for (int i = 0; i < 3; ++i)
		{
			XMVECTOR vScale;
			XMVECTOR vRot;
			XMVECTOR vTrans;
			XMMatrixDecompose(&vScale, &vRot, &vTrans, m_matWorld);
			Vec3 Scale = vScale;
			Matrix matScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(Scale.x, Scale.y, Scale.z));

			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], matScaleInv * m_matWorld);
			m_WorldDir[i].Normalize();
		}
	}
}

void CTransform::Binding()
{
	// ������Ʈ�� ��ġ�� ������۷� �̵���Ų��.
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_Trans.matWorld = m_matWorld;
	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	pTransformBuffer->SetData(&g_Trans, sizeof(tTransform));
	pTransformBuffer->Binding();
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_RelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	bool bIndependent = m_IndependentScale;

	while (pParent && !bIndependent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		bIndependent = pParent->Transform()->m_IndependentScale;
		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

Vec3 CTransform::GetRelativeRotation()
{
	Vec3 vRotation = (m_RelativeRot / XM_PI) * 180.f;
	return vRotation;
}

void CTransform::SetRelativeRotation(Vec3 _Rotation)
{
	m_RelativeRot = (_Rotation / 180.f) * XM_PI;
}

void CTransform::SetRelativeRotation(float _x, float _y, float _z)
{
	m_RelativeRot = (Vec3(_x, _y, _z) / 180.f) * XM_PI;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);	
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRot, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}
