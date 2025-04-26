#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(Type) class C##Type* Type() { return m_Owner->Type(); }

class CComponent :
    public CEntity
{
private:
    const COMPONENT_TYPE    m_Type;
    CGameObject*            m_Owner;

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

protected:
    GET_OTHER_COMPONENT(Transform);    
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Light2D);

    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(SpriteRender);
    GET_OTHER_COMPONENT(FlipbookRender);
    GET_OTHER_COMPONENT(TileRender);
    GET_OTHER_COMPONENT(ParticleRender);

    class CRenderComponent* RenderComponent() { return m_Owner->GetRenderComponent(); }

public:
    virtual void Init() {}
    virtual void Begin() {};
    virtual void Tick() {};
    virtual void FinalTick() = 0;

    // 컴포넌트 정보가 파일(레벨)에 저장 / 불러올때 필수로 저장해야 하는 내용을 작성
    virtual void SaveToFile(FILE* _File) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;

public:
    virtual CComponent* Clone() = 0;

public:
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _Origin);
    ~CComponent();
    friend class CGameObject;
};

