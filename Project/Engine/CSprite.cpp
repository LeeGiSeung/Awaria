#include "pch.h"
#include "CSprite.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"

CSprite::CSprite(bool _EngineAsset)
    : CAsset(ASSET_TYPE::SPRITE, _EngineAsset)
{
}

CSprite::~CSprite()
{
}

void CSprite::SetLeftTop(Vec2 _LeftTopPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    } 

    m_LeftTop = _LeftTopPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CSprite::SetSlice(Vec2 _SlicePixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }

    m_Slice = _SlicePixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

void CSprite::SetOffset(Vec2 _OffsetPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }
    m_Offset = _OffsetPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
    m_intOffset = _OffsetPixel;
}

void CSprite::SetBackground(Vec2 _BackgroundPixel)
{
    if (nullptr == m_Atlas)
    {
        m_LeftTop = Vec2(0.f, 0.f);
        m_Slice = Vec2(0.f, 0.f);
        m_Background = Vec2(0.f, 0.f);
        m_Offset = Vec2(0.f, 0.f);
        return;
    }

    m_Background = _BackgroundPixel / Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
}

int CSprite::Save(const wstring& _FilePath)
{
    //SetRelativePathByFilePath(_FilePath);    

    //FILE* pFile = nullptr;
    //_wfopen_s(&pFile, _FilePath.c_str(), L"wb");


    //SaveAssetRef(m_Atlas, pFile);

    //fwrite(&m_LeftTop, sizeof(Vec2), 1, pFile);
    //fwrite(&m_Slice, sizeof(Vec2), 1, pFile);
    //fwrite(&m_Offset, sizeof(Vec2), 1, pFile);
    //fwrite(&m_Background, sizeof(Vec2), 1, pFile);

    //fclose(pFile);

    //wstring RelativePath = L"Sprite\\" + _FilePath; //원래 더했는데 FlipbookEditor때문에 뺌
    wstring RelativePath = _FilePath;
    CheckExt(L".sprite", RelativePath);

    // 에셋이 자신이 어디에 저장되는지를 알게 함
    SetRelativePath(RelativePath);

    wstring strFilePath = CPathMgr::GetContentPath() + RelativePath;

    FILE* File = nullptr;
    _wfopen_s(&File, strFilePath.c_str(), L"w");

    // 가리키던 Atlas 텍스쳐의 키, 경로를 저장한다.	
    fwprintf_s(File, L"[KEY]\n");
    fwprintf_s(File, L"%s\n\n", GetKey().c_str());

    fwprintf_s(File, L"[PATH]\n");
    fwprintf_s(File, L"%s\n\n", GetRelativePath().c_str());

    wstring strAtlasKey, strAtlasPath;

    if (m_Atlas != nullptr)
    {
        strAtlasKey = m_Atlas->GetKey();
        strAtlasPath = m_Atlas->GetRelativePath();
    }
    fwprintf_s(File, L"[ATLAS_KEY]\n");
    fwprintf_s(File, L"%s\n", strAtlasKey.c_str());
    //fwprintf_s(File, L"%s\n", strAtlasPath.c_str());
    fwprintf_s(File, L"[ATLAS_PATH]\n");
    fwprintf_s(File, L"%s\n\n", strAtlasPath.c_str());

    m_intLeftTop.x = denormalize(m_LeftTop.x, 0.0f, m_Atlas->GetWidth());
    m_intLeftTop.y = denormalize(m_LeftTop.y, 0.0f, m_Atlas->GetHeight());
    m_intSlice.x = denormalize(m_Slice.x, 0.0f, m_Atlas->GetWidth());
    m_intSlice.y = denormalize(m_Slice.y, 0.0f, m_Atlas->GetHeight());
    m_intOffset.x = denormalize(m_Offset.x, 0.0f, m_Atlas->GetWidth());
    m_intOffset.y = denormalize(m_Offset.y, 0.0f, m_Atlas->GetHeight());


    fwprintf_s(File, L"[LEFT_TOP]\n");
    fwprintf_s(File, L"%d, %d\n\n", (int)m_intLeftTop.x, (int)m_intLeftTop.y);

    fwprintf_s(File, L"[SLICE]\n");
    fwprintf_s(File, L"%d, %d\n\n", (int)m_intSlice.x, (int)m_intSlice.y);

    fwprintf_s(File, L"[OFFSET]\n");
    fwprintf_s(File, L"%d, %d\n\n", (int)m_intOffset.x, (int)m_intOffset.y);

    fwprintf_s(File, L"[BACKGROUND]\n");
    fwprintf_s(File, L"%f, %f\n\n", m_Background.x, m_Background.y);

    fclose(File);

    return S_OK;
}

int CSprite::Load(const wstring& _FilePath)
{
    //FILE* pFile = nullptr;
    //_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

    //// 파일 스트림 열기 실패 시
    //if (nullptr == pFile)
    //    return E_FAIL;

    //m_Atlas = LoadAssetRef<CTexture>(pFile);

    //fread(&m_LeftTop, sizeof(Vec2), 1, pFile);
    //fread(&m_Slice, sizeof(Vec2), 1, pFile);
    //fread(&m_Offset, sizeof(Vec2), 1, pFile);
    //fread(&m_Background, sizeof(Vec2), 1, pFile);

    //fclose(pFile);

    //wstring file_path = L"Sprite\\" + _FilePath;

    wstring strFilePath = _FilePath;

    FILE* File = nullptr;
    if (_wfopen_s(&File, strFilePath.c_str(), L"r") == true) {
        int a = 0;
    };

    // 가리키던 Atlas 텍스쳐의 키, 경로를 저장한다.
    wstring AtlasKey, AtlasPath;

    while (true)
    {
        wchar_t szBuff[255] = {};
        if (EOF == fwscanf_s(File, L"%s", szBuff, 255))
        {
            break;
        }

        if (!wcscmp(szBuff, L"[KEY]"))
        {
            fwscanf_s(File, L"%s", szBuff, 255);
            SetKey(szBuff);
        }

        else if (!wcscmp(szBuff, L"[PATH]"))
        {
            fwscanf_s(File, L"%s", szBuff, 255);
            SetRelativePath(szBuff);
        }

        else if (!wcscmp(szBuff, L"[ATLAS_KEY]"))
        {
            fwscanf_s(File, L"%s", szBuff, 255);
            AtlasKey = szBuff;
        }

        else if (!wcscmp(szBuff, L"[ATLAS_PATH]"))
        {
            fwscanf_s(File, L"%s", szBuff, 255);
            AtlasPath = szBuff;
            m_Atlas = CAssetMgr::GetInst()->Load<CTexture>(AtlasKey, AtlasPath);
        }

        else if (!wcscmp(szBuff, L"[LEFT_TOP]"))
        {
            int x = 0, y = 0;
            fwscanf_s(File, L"%d, %d", &x, &y);
            //m_LeftTop = Vec2(x, y);
            m_LeftTop = Vec2(normalizea(x, 0.0f, m_Atlas->GetWidth()), normalizea(y, 0.0f, m_Atlas->GetHeight()));
        }

        else if (!wcscmp(szBuff, L"[SLICE]"))
        {
            int x = 0, y = 0;
            fwscanf_s(File, L"%d, %d", &x, &y);
            //m_Slice = Vec2(x, y);
            m_Slice = Vec2(normalizea(x, 0.0f, m_Atlas->GetWidth()), normalizea(y, 0.0f, m_Atlas->GetHeight()));
        }

        else if (!wcscmp(szBuff, L"[OFFSET]"))
        {
            int x = 0, y = 0;
            fwscanf_s(File, L"%d, %d", &x, &y);
            m_intOffset = Vec2(x,y);
            //m_Offset = Vec2(x, y);
            m_Offset = Vec2(normalizea(x, 0.0f, m_Atlas->GetWidth()), normalizea(y, 0.0f, m_Atlas->GetHeight()));

        }
        else if (!wcscmp(szBuff, L"[BACKGROUND]"))
        {
            float x = 0, y = 0;
            fwscanf_s(File, L"%f, %f", &x, &y);
            m_Background = Vec2(x, y);
        }
    }

    fclose(File);

    return S_OK;
}