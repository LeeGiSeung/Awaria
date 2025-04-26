#pragma once

#include <Engine/CScript.h>

class CCollider2D;
class CGameObject;

class CPlayerScript :
    public CScript
{
private:
    float               m_Speed;
    float               m_PaperBurn;

    //PlayerScript가 사용할 Prefab
    Ptr<CPrefab>        m_Prefab;
    int                 CUR_PLAYER_STATE;
    CGameObject* m_PlayerObject;

    bool                m_PlayerMoveCheckBool;

    int                 PlayerHP;
    bool                PlayerHP_Drone;
    float               Drone_CoolTime;

    bool                Invincibility;
    bool                Dash_Invincibility;
    float               Dash_Invincibility_Time;

    float               Invincibility_Time;


    //팔, 다리 , 머리 흔들기
    int                 PastSpriteIdx;
    int                 PastFlipIdx;
    int                 PastDir;
    int                 Pngidx;

    PART                LEFTHAND;
    PART                RIGHTHAND;

    //벽 이동
    MOVE_DIR  m_PlayerUDDir;
    MOVE_DIR  m_PlayerLRDir;

    bool m_bBlockLeft;
    bool m_bBlockRight;
    bool m_bBlockUp;
    bool m_bBlockDown;

    int bBlockCount;

    PNG_PLAYER_MOVE     Png_move;

    //캐릭터 왼손, 오른손 들고있는 오브젝트
    PART LEFT_HAND;
    PART RIGHT_HAND;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _Other, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

