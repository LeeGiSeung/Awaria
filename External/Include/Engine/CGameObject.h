#pragma once

#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;


class CGameObject :
    public CEntity
{
private:
    CComponent*             m_Com[COMPONENT_END];
    vector<CScript*>        m_vecScript;
    CRenderComponent*       m_RenderCom; // 렌더링 컴포넌트 포인터

    CGameObject*            m_Parent;   // 부모 오브젝트
    vector<CGameObject*>    m_vecChild; // 자식 오브젝트

    int                     m_LayerIdx; // 소속 레이어 인덱스, -1 : 어느 레이어에도 소속되어있지 않다.
    bool                    m_Dead;  // 오브젝트가 삭제되기 직전 상태 체크

    APPARATUS_STATE         App_state;
    bool Tag;
    bool SpawnPart;
    bool UISpawnPart;
    bool GeneratorUIUpdate;
    float RunningTime;

    float ShoatAngle;

    bool want;

    GENERATOR_UI_STATE m_Generator_ui_state;
    PART Have_Part1;
    PART Have_Part2;

    int  RecentHand;

    PART Want_Part1;
    PART Want_Part2;

    bool GeneratorCheck1;
    bool GeneratorCheck2;

    bool            PlayerHeat;

    int             RepairCount;

public:

    void SetRepairCountUp() { RepairCount++; }
    int GetRepairCount() { return RepairCount; }

    void SetShoatAngle(float _in) { ShoatAngle = _in; }
    float GetShoatAngle() { return ShoatAngle; }

    void SetPlayerHeat(bool _in) { PlayerHeat = _in; }
    bool GetPlayerHeat() { return PlayerHeat; }

    bool GetWant() { return want; }

    void SetWant1(bool _in) { want = _in; }

    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_Com[_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    APPARATUS_STATE GetAppState() { return App_state; }

    void SetRecentHand(int _in) { RecentHand = _in; }
    int GetRecentHand() { return RecentHand; }

    void SetTag(bool _in) { Tag = _in; }
    bool GetTag() { return Tag; }
    void SetAppState(APPARATUS_STATE _in) { App_state = _in; }
    void SetSpawnPart(bool _in) { SpawnPart = _in; }

    void SetRunningTime(float _in) { RunningTime = _in; }
    float GetRunningTime() { return RunningTime; }

    void SetGenerator_ui_state(GENERATOR_UI_STATE _in) { m_Generator_ui_state = _in; }
    GENERATOR_UI_STATE GetGenerator_ui_state() {
        return m_Generator_ui_state;
    }

    bool GetGeneratorUIUpdate() { return GeneratorUIUpdate; }
    void SetGeneratorUIUpdate(bool _in) { GeneratorUIUpdate = _in; }

    void SetHavePart1(PART _in) { Have_Part1 = _in; }
    void SetHavePart2(PART _in) { Have_Part2 = _in; }
    PART GetHavePart1() { return Have_Part1; }
    PART GetHavePart2() { return Have_Part2; }
    void SetWantPart1(PART _in) { Want_Part1 = _in; }
    void SetWantPart2(PART _in) { Want_Part2 = _in; }
    PART GetWantPart1() { return Want_Part1; }
    PART GetWantPart2() { return Want_Part2; }

    void SetGeneratorCheck1(bool _in) { GeneratorCheck1 = _in; }
    void SetGeneratorCheck2(bool _in) { GeneratorCheck2 = _in; }

    bool GetGeneratorCheck1() { return GeneratorCheck1; }
    bool GetGeneratorCheck2() { return GeneratorCheck2; }

    bool GetUISpawnPart() { return UISpawnPart; }
    void SetUiSpawnPart(bool _in) { UISpawnPart = _in; }

    bool GetSpawnPart() { return SpawnPart; }
    int GetLayerIndex() { return m_LayerIdx; }

    bool IsDead() { return m_Dead; }
    bool IsAncestor(CGameObject* _Object);

    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Child);
     
    CGameObject* GetParent() const { return m_Parent; }
    const vector<CGameObject*>& GetChild() const { return m_vecChild; } // 자식 오브젝트
    const vector<CScript*>& GetScripts() { return m_vecScript; }

    class CTransform* Transform() { return (CTransform*)GetComponent(TRANSFORM); }    
    class CCamera* Camera() { return (CCamera*)GetComponent(CAMERA); }
    class CCollider2D* Collider2D() { return (CCollider2D*)GetComponent(COLLIDER2D); }
    class CLight2D* Light2D() { return (CLight2D*)GetComponent(LIGHT2D); }

    class CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(MESHRENDER); }
    class CSpriteRender* SpriteRender() { return (CSpriteRender*)GetComponent(SPRITERENDER); }
    class CFlipbookRender* FlipbookRender() { return (CFlipbookRender*)GetComponent(FLIPBOOKRENDER); }
    class CTileRender* TileRender() { return (CTileRender*)GetComponent(TILERENDER); }
    class CParticleRender* ParticleRender() { return (CParticleRender*)GetComponent(PARTICLERENDER); }

    void Destroy();


public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

private:
    void DisconnectWithParent();
    void RegisterAsParent();
    void DisconnectWithLayerAsParent();

public:
    CLONE(CGameObject);

public:
    CGameObject();
    ~CGameObject();

protected:
    CGameObject(const CGameObject& _Origin);

    friend class CLayer;
    friend class CTaskMgr;
    friend class CPrefab;
};

