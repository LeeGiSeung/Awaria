#include "pch.h"
#include "CAsset.h"

#include "CPathMgr.h"

CAsset::CAsset(ASSET_TYPE _Type, bool _EngineAsset)
	: m_Type(_Type)
	, m_EngineAsset(_EngineAsset)
{
}

CAsset::~CAsset()
{
}

void CAsset::CheckExt(const wstring& _Ext, wstring& _strFilePath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	// 확장자가 올바르지 않다면
	if (wcscmp(szExt, _Ext.c_str()))
	{
		// 확장자가 세팅되어있지 않았다면
		if (!wcscmp(szExt, L""))
		{
			_strFilePath += _Ext;
		}

		// 확장자가 잘못 세팅되어있다면
		else
		{
			_strFilePath = _strFilePath.substr(0, _strFilePath.find(szExt));
			_strFilePath += _Ext;
		}
	}
}

void CAsset::SetRelativePathByFilePath(const wstring& _FullPath)
{
	// 입력된 전체경로에서 ContentPath 까지의 경로를 덜어내고, 상대경로만 잘라내서 설정해준다.
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring RelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());
	SetRelativePath(RelativePath);
}