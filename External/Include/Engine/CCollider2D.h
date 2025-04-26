#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec3    m_Scale;
    Vec3    m_Offset;

    int     m_OverlapCount;
    bool    m_IndependentScale;
    bool    m_NotifyParent;
    bool    m_ShowColor;

    Matrix  m_matColWorld;
    

public:
    void SetScale(Vec2 _Scale) { m_Scale.x = _Scale.x; m_Scale.y = _Scale.y; }
    void SetOffset(Vec2 _Offset) { m_Offset.x = _Offset.x; m_Offset.y = _Offset.y; }

    Vec2 GetScale() { return Vec2(m_Scale.x, m_Scale.y); }
    Vec2 GetOffset() { return Vec2(m_Offset.x, m_Offset.y); }

    const Matrix& GetWorldMat() { return m_matColWorld; }

    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set;}
    bool GetIndependentScale() { return m_IndependentScale; }

    void SetNotifyParent(bool _Notify) { m_NotifyParent = _Notify; }

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:
    void BeginOverlap(CCollider2D* _Other);
    void Overlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

private:
    void NotifyBeginOverlap(CCollider2D* _Other);
    void NotifyOverlap(CCollider2D* _Other);
    void NotifyEndOverlap(CCollider2D* _Other);

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

