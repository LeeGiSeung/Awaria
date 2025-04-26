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
	//**라디안(rad)**으로 변환하는 이유는, C++에서 제공하는 삼각함수(cos, sin 등) 들이 각도를 라디안 단위로 계산하기 때문이야.
	// 각도를 라디안으로 변환
	float rad = Angle * XM_PI / 180.f;

	// 방향벡터 계산 (정규화된 벡터)
	m_vDir.x = cosf(rad);
	m_vDir.y = sinf(rad);
	m_vDir.z = -5; // 필요시

	// 이동
	vPos += m_vDir * m_Speed * DT;

	// Cos활용을 위해 라디안으로 변환
	float degToRad = m_CurObject->GetShoatAngle() * XM_PI / 180.f;

	// 이동 방향 계산
	Vec3 direction = {
		cosf(degToRad),
		sinf(degToRad),
		vPos.z
	};

	// 방향 벡터 정규화
	direction.Normalize();

	// 이동
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
	//코뿔소와 Player가 충돌했을 당시
	if (_Other->GetName() == L"pBody") {
		//코뿔소가 공격 상태일때

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


