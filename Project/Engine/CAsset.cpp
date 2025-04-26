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

	// Ȯ���ڰ� �ùٸ��� �ʴٸ�
	if (wcscmp(szExt, _Ext.c_str()))
	{
		// Ȯ���ڰ� ���õǾ����� �ʾҴٸ�
		if (!wcscmp(szExt, L""))
		{
			_strFilePath += _Ext;
		}

		// Ȯ���ڰ� �߸� ���õǾ��ִٸ�
		else
		{
			_strFilePath = _strFilePath.substr(0, _strFilePath.find(szExt));
			_strFilePath += _Ext;
		}
	}
}

void CAsset::SetRelativePathByFilePath(const wstring& _FullPath)
{
	// �Էµ� ��ü��ο��� ContentPath ������ ��θ� �����, ����θ� �߶󳻼� �������ش�.
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring RelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());
	SetRelativePath(RelativePath);
}