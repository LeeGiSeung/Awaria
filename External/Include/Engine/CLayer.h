#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParentObjects; // ���̾ �Ҽӵ� ������Ʈ ��, �ֻ��� �θ� ������Ʈ�� ����
    vector<CGameObject*>    m_vecObjects;       // �ֻ��� �θ�, �ڽ� �������, �ش� ���̾� �Ҽ� ��� ������Ʈ
    int                     m_LayerIdx;         // ���̾� ��ȣ

public:
    void Begin();
    void Tick();
    void FinalTick();    

public:
    void AddGameObject(CGameObject* _Parent, bool _bChildMove);
    void RegisterGameObject(CGameObject* _Object) { m_vecObjects.push_back(_Object); }

    // �ش� ���̾� �Ҽ� ������Ʈ�߿��� �ֻ��� �θ� ���̻� �ƴϰ� �� ������Ʈ�� ����� ����Ѵ�.
    void DeregisterGameObjectAsParent(CGameObject* _Object);


    const vector<CGameObject*>& GetParentObjects() { return m_vecParentObjects; }
    const vector<CGameObject*>& GetObjects() { return m_vecObjects; }

public:
    CLONE(CLayer);

public:
    CLayer();    
    ~CLayer();

private:
    CLayer(const CLayer& _Origin);

    friend class CLevel;
};

