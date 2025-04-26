#include "pch.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_ContentPath[255]	= {};
wchar_t CPathMgr::g_BinPath[255]		= {};
wchar_t CPathMgr::g_SolutionPath[255]	= {};

void CPathMgr::Init()
{
	wchar_t szBuff[255] = {};

	// ���� ���μ����� ���(����� ���� ������ ��� ����ó�� �ʿ�, ������Ʈ ����->Debug->�۾����͸� ����
	GetCurrentDirectory(255, szBuff);

	// Bin ��� ����
	wcscpy_s(g_BinPath, 255, szBuff);
	wcscat_s(g_BinPath, 255, L"\\");

	// Bin ����������� ���
	for (int i = wcslen(szBuff); 0 <= i; --i)
	{
		if (L'\\' == szBuff[i])
		{
			szBuff[i] = L'\0';
			break;
		}
	}

	// �ڿ� Content ���̱�
	wcscat_s(szBuff, 255, L"\\Content\\");
	wcscpy_s(g_ContentPath, 255, szBuff);
}

wstring CPathMgr::GetRelativePath(const wstring& _FilePath)
{
	wstring contentPath = GetContentPath();
	
	if (-1 == _FilePath.find(contentPath.c_str()))
	{
		return wstring();
	}
	
	return _FilePath.substr(contentPath.length(), _FilePath.length());
}
