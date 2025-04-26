#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_Mesh;

    Ptr<CMaterial>          m_SharedMtrl;   // AssetMgr �κ��� ����
    Ptr<CMaterial>          m_CurMtrl;      // RenderComponent ������ ���� ������� ����

    // RenderComponent ��ü ���θ� ����� ���� ����(AssetMgr �� ��Ͻ�Ű�� �ʴ´�.)
    // �������� ������ ���� ������ ���°� Play ������ ��쿡�� ��� ����
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

