#pragma once
#include "CAsset.h"


// 1. �۾��� ���꼺 �ø���.
// 2. ���� �÷��� ���� �ε��� ���� �� �ִ�.
//   - ��ü�� ���� �����Ҷ� �߻��ϴ� ���ҽ� �ε��� �̸� �����ص�
// Prototype Pattern


typedef void(*GAMEOBJECT_SAVE)(CGameObject*, FILE*);
typedef CGameObject*(*GAMEOBJECT_LOAD)(FILE*);

class CPrefab :
    public CAsset
{
public:
    static GAMEOBJECT_SAVE  g_Save;
    static GAMEOBJECT_LOAD  g_Load;

private:
    CGameObject*    m_PrefabObject;
      
public:
    void SetGameObject(CGameObject* _Object);
    CGameObject* Instantiate();

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

    CLONE_DISABLE(CPrefab);

public:  
    CPrefab(bool _EngineAsset = false);
    ~CPrefab();
};

