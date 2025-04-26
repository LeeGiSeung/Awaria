#include "pch.h"

#include "CRenderMgr.h"

void DrawDebugRect(Vec4 _Color, Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation
	             , bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.WorldPos = _WorldPos;
	info.WorldScale = _WorldScale;
	info.WorldRotation = _WorldRotation;
	info.MatWorld = XMMatrixIdentity();
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::RECT;
	info.Color = _Color;	
	info.MatWorld = _matWorld;
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec4 _Color, Vec3 _WorldPos, float _Radius, Vec3 _WorldRotation
	               , bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};
	info.Shape = DEBUG_SHAPE::CIRCLE;
	info.Color = _Color;
	info.WorldPos = _WorldPos;
	info.WorldScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	info.WorldRotation = _WorldRotation;
	info.CurTime = 0.f;
	info.Duration = _Duration;
	info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugLine(Vec4 _Color, Vec3 _Start, Vec3 _End)
{

}

#include "CTaskMgr.h"
void SpawnGameObject(CGameObject* _Object, int _LayerIdx)
{
	tTask task = {};

	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param0 = (DWORD_PTR)_Object;
	task.Param1 = (DWORD_PTR)_LayerIdx;

	CTaskMgr::GetInst()->AddTask(task);
}

string ToString(const wstring& _String)
{
	return string(_String.begin(), _String.end());	
}

wstring ToWString(const string& _String)
{
	return wstring(_String.begin(), _String.end());
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	size_t len = _Str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), len, _File);
}

void LoadWString(wstring& _Str, FILE* _File)
{
	wchar_t Buff[255] = {};

	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);
	fread(Buff, sizeof(wchar_t), len, _File);

	_Str = Buff;
}
