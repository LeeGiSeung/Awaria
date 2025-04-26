#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_RelativePos;          // ��ġ
    Vec3    m_RelativeScale;        // ũ��
    Vec3    m_RelativeRot;          // ȸ��

    Vec3    m_RelativeDir[(UINT)DIR::END];  // ������Ʈ�� ��������
    Vec3    m_WorldDir[(UINT)DIR::END];     // ������Ʈ�� ��������

    Matrix  m_matWorld;     // ���� ��ȯ���

    // �ڽ� ������Ʈ�϶� �θ��� ũ�⿡ ������ �����ʴ� ������ ũ�⸦ ������.
    bool    m_IndependentScale; 


public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Binding();   

public:
    void SetRelativePos(Vec3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativePos(float _x, float _y, float _z) { m_RelativePos = Vec3(_x, _y, _z); }

    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeScale(float _x, float _y, float _z) { m_RelativeScale = Vec3(_x, _y, _z); }

    void SetRelativeRotation(Vec3 _Rotation);
    void SetRelativeRotation(float _x, float _y, float _z);

    bool IsIndependentScale() { return m_IndependentScale; }
    void SetIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }

    Vec3 GetRelativePos() { return m_RelativePos; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }

    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3 GetWorldScale();

    Vec3 GetRelativeRotation();
    Vec3 GetWorldRotation();

    Vec3 GetRelativeDir(DIR _DirType) { return m_RelativeDir[(UINT)_DirType]; }
    Vec3 GetWorldDir(DIR _DirType) { return m_WorldDir[(UINT)_DirType]; }

    const Matrix& GetWorldMat() { return m_matWorld; }
    void SetWorldMat(const Matrix& _Mat) { m_matWorld = _Mat; }

public:
    CLONE(CTransform);

public:
    CTransform();
    ~CTransform();
};

