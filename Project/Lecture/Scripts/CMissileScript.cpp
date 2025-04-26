#include "pch.h"
#include "CMissileScript.h"

#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>

CMissileScript::CMissileScript()
	: CScript(MISSILESCRIPT)
	, m_vDir(0.f, 1.f, 0.f)
	, m_Speed(700.f)
{
}

CMissileScript::~CMissileScript()
{

}


void CMissileScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	CGameObject* m_CurObject = this->GetOwner();

	float Angle = m_CurObject->GetShoatAngle();
	//**����(rad)**���� ��ȯ�ϴ� ������, C++���� �����ϴ� �ﰢ�Լ�(cos, sin ��) ���� ������ ���� ������ ����ϱ� �����̾�.
	// ������ �������� ��ȯ
	float rad = Angle * XM_PI / 180.f;

	// ���⺤�� ��� (����ȭ�� ����)
	m_vDir.x = cosf(rad);
	m_vDir.y = sinf(rad);
	m_vDir.z = -5; // �ʿ��

	// �̵�
	vPos += m_vDir * m_Speed * DT;

	// CosȰ���� ���� �������� ��ȯ
	float degToRad = m_CurObject->GetShoatAngle() * XM_PI / 180.f;

	// �̵� ���� ���
	Vec3 direction = {
		cosf(degToRad),
		sinf(degToRad),
		vPos.z
	};

	// ���� ���� ����ȭ
	direction.Normalize();

	// �̵�
	vPos += direction * m_Speed * DT;

	Transform()->SetRelativePos(vPos);
	float z = Transform()->GetRelativeRotation().z + 1.f;
	float x = Transform()->GetRelativeRotation().x;
	float y = Transform()->GetRelativeRotation().y;
	Transform()->SetRelativeRotation(x, y, z);
}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider)
{
	CGameObject* m_CurObject = this->GetOwner();
	//�ڻԼҿ� Player�� �浹���� ���
	if (_Other->GetName() == L"pBody") {
		//�ڻԼҰ� ���� �����϶�

		_Other->SetPlayerHeat(true);

	}
	else if (L"pWall" == _Other->GetName())
	{
		m_CurObject->Destroy();
	}
}

void CMissileScript::SaveToFile(FILE* _File)
{
	fwrite(&m_vDir, sizeof(Vec2), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CMissileScript::LoadFromFile(FILE* _File)
{
	fread(&m_vDir, sizeof(Vec2), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
}


