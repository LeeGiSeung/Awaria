#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    PROJ_TYPE               m_ProjType;     // 투영 방법
    float                   m_OrthoScaleX;  // 직교투영 가로길이
    float                   m_AspectRatio;  // 가로 세로 종횡비
    float                   m_FOV;          // 시야각(FieldOfView)
    float                   m_Far;          // 최대 시야거리

    Matrix                  m_matView;      // View 행렬
    Matrix                  m_matProj;      // 투영행렬

    int                     m_Priority;     // 카메라 우선순위, -1 : 미등록 카메라, 0 : 메인 카메라, 1 ~> : 서브 카메라

    UINT                    m_LayerCheck;   // 카메라가 렌더링할 레이어 비트설정

    // 물체 분류 용도
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMasked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;


public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetOrthoScaleX(float _Scale) { m_OrthoScaleX = _Scale; }
    void SetAspectRatio(float _Ratio) { m_AspectRatio = _Ratio; }
    void SetFOV(float _FOV) { m_FOV = (_FOV / 180.f) * XM_PI; }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetPriority(int _Priority) { m_Priority = _Priority; }

    PROJ_TYPE GetProjType() { return m_ProjType; }
    float GetOrthoScaleX() { return m_OrthoScaleX; }
    float GetAspectRatio() { return m_AspectRatio; }
    float GetFar() { return m_Far; }
    float GetFOV() { return (m_FOV / XM_PI) * 180.f; }
    int GetPriority() { return m_Priority; }

    void CheckLayer(UINT _LayerIdx)
    {
        /*if (m_LayerCheck & (1 << _LayerIdx))
        {
            m_LayerCheck &= ~(1 << _LayerIdx);
        }
        else
        {
            m_LayerCheck |= ~(1 << _LayerIdx);
        }*/
        m_LayerCheck ^= (1 << _LayerIdx);
    }
    void CheckLayerAll() { m_LayerCheck = 0xffffffff; }
    void CheckLayerClear() { m_LayerCheck = 0; }

    const Matrix& GetProjMat() { return m_matProj; }

private:
    void SortObject();

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Render();

    CLONE(CCamera);
public:
    CCamera();
    CCamera(const CCamera& _Origin);
    ~CCamera();

    friend class CRenderMgr;
};