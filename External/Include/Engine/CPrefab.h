#pragma once
#include "CAsset.h"


// 1. 작업의 생산성 올린다.
// 2. 게임 플레이 도중 로딩을 줄일 수 있다.
//   - 객체가 최초 등장할때 발생하는 리소스 로딩을 미리 수행해둠
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

