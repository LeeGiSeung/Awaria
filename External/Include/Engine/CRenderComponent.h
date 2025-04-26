#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;

    Ptr<CMaterial>          m_SharedMtrl;   // AssetMgr 로부터 얻어옴
    Ptr<CMaterial>          m_CurMtrl;      // RenderComponent 본인이 현재 사용중인 재질

    // RenderComponent 객체 본인만 사용할 전용 재질(AssetMgr 에 등록시키지 않는다.)
    // 동적재질 생성은 현재 레벨의 상태가 Play 상태일 경우에만 사용 가능
    Ptr<CMaterial>          m_DynamicMtrl;


public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    Ptr<CMesh> GetMesh() { return m_Mesh; }

    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMaterial> GetMaterial() { return m_CurMtrl; }

    Ptr<CMaterial> GetSharedMaterial() 
    {
        m_CurMtrl = m_SharedMtrl;
        return m_SharedMtrl;
    }

    Ptr<CMaterial> GetDynamicMaterial();

    virtual void Render() = 0;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

