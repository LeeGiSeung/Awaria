#pragma once


class CLevel;
class CGameObject;

class CLevelSaveLoad
{
public:

	static void SaveLevel(wstring _FilePath);
	static CLevel* LoadLevel(wstring _FilePath);
	static void SaveGameObject(CGameObject* _Object, FILE* _File);
	static CGameObject* LoadGameObject(FILE* _File);

private:
	CLevelSaveLoad() {}
	~CLevelSaveLoad(){}
};

