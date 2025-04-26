#pragma once
#include <Engine/CGameObject.h>

class CGameObjectEX :
    public CGameObject
{
public:
    void FinalTick();



public:
    CLONE(CGameObjectEX);
    CGameObjectEX();
    ~CGameObjectEX();
};

