#pragma once

void DrawDebugRect(Vec4 _Color, Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WorldRotation, bool _DepthTest = true, float _Duration = 0.f);
void DrawDebugRect(Vec4 _Color, const Matrix& _matWorld, bool _DepthTest = true, float _Duration = 0.f);

void DrawDebugCircle(Vec4 _Color, Vec3 _WorldPos, float _Radius, Vec3 _WorldRotation, bool _DepthTest = true, float _Duration = 0.f);
void DrawDebugLine(Vec4 _Color, Vec3 _Start, Vec3 _End);

void SpawnGameObject(class CGameObject* _Object, int _LayerIdx);


string ToString(const wstring& _String);
wstring ToWString(const string& _String);

void SaveWString(const wstring& _Str, FILE* _File);
void LoadWString(wstring& _Str, FILE* _File);




template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
			//_vec[i] = nullptr;
		}

		
	}

	_vec.clear();
}
