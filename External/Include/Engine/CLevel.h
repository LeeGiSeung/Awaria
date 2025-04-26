#pragma once
#include "CEntity.h"


class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*     m_Layer[MAX_LAYER];
    LEVEL_STATE m_State;

public:
    void Begin();
    void Tick();
    void FinalTick();  

public:
    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    LEVEL_STATE GetState() { return m_State; }
    void ChangeState(LEVEL_STATE _NextState);
    void Deregister();

public:
    void AddGameObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove);
    CGameObject* FindObjectByName(const wstring& _Name);



public:
    CLONE(CLevel);

public:
    CLevel();
    ~CLevel();

private:
    CLevel(const CLevel& _Origin);
};

