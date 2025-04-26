#pragma once
#include "EditorUI.h"
#include <array>

class GameObject;

class ChangeToPrefab :
	public EditorUI
{
public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

    bool PreFabIndex = 0;

    bool MakePrefab = false;
    bool DeleteObject = false;

    int DeleteObjectIndex = -1;
    int selectedIndex = -1;

    string PrefabSTR;

    string DeleteSTR;
    vector<const char*> charPtrList;

public:
    ChangeToPrefab();
    ~ChangeToPrefab();
};

