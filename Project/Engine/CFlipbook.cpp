#include "pch.h"
#include "CFlipbook.h"

#include "CPathMgr.h"
#include "CAssetMgr.h"

CFlipbook::CFlipbook(bool _EngineAsset)
    : CAsset(ASSET_TYPE::FLIPBOOK, _EngineAsset)
{
}

CFlipbook::~CFlipbook()
{
}


int CFlipbook::Save(const wstring& _FilePath)
{
    //FILE* pFile = nullptr;
    //_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

    //// 스프라이트 개수
    //size_t Count = m_vecSprite.size();
    //fwrite(&Count, sizeof(size_t), 1, pFile);

    //// 스프라이트 각각 정보
    //for (size_t i = 0; i < m_vecSprite.size(); ++i)
    //{
    //    SaveAssetRef(m_vecSprite[i], pFile);
    //}

    //fclose(pFile);

    wstring RelativePath = _FilePath;
    CheckExt(L".flip", RelativePath);

    // 에셋이 자신이 어디에 저장되는지를 알게 함
    SetRelativePath(RelativePath);

    // 최종 저장 경로
    wstring strFilePath = CPathMgr::GetContentPath() + RelativePath;

    // Sprite 의 Key, RelativePath 를 저장
    FILE* File = nullptr;
    _wfopen_s(&File, strFilePath.c_str(), L"w");

    // KEY, Path 저장
    fwprintf_s(File, L"[KEY]\n");
    fwprintf_s(File, L"%s\n", GetKey().c_str());

    fwprintf_s(File, L"[PATH]\n");
    fwprintf_s(File, L"%s\n\n", GetRelativePath().c_str());

    // Sprite 개수
    fwprintf_s(File, L"[SPRITE_COUNT]\n");
    int SpriteCount = (int)m_vecSprite.size();
    fwprintf_s(File, L"%d\n", SpriteCount);

    // 각각의 Sprite 정보 저장
    for (int i = 0; i < (int)m_vecSprite.size(); ++i)
    {
        fwprintf_s(File, L"[INDEX]\n");
        fwprintf_s(File, L"%d\n", i);
        fwprintf_s(File, L"[KEY]\n");
        fwprintf_s(File, L"%s\n", m_vecSprite[i]->GetKey().c_str());
        fwprintf_s(File, L"[PATH]\n");
        fwprintf_s(File, L"%s\n", m_vecSprite[i]->GetKey().c_str());
        //fwprintf_s(File, L"%s\n\n", m_vecSprite[i]->GetRelativePath().c_str());

    }

    fclose(File);

    return S_OK;
}

int CFlipbook::Load(const wstring& _FilePath)
{
    //SetRelativePathByFilePath(_FilePath);

    //FILE* pFile = nullptr;
    //_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

    //// 파일 스트림 열기 실패 시
    //if (nullptr == pFile)
    //    return E_FAIL;

    //// 스프라이트 개수
    //size_t Count = 0;
    //fread(&Count, sizeof(size_t), 1, pFile);

    //// 스프라이트 각각 정보
    //for (size_t i = 0; i < Count; ++i)
    //{
    //    AddSprite(LoadAssetRef<CSprite>(pFile));        
    //}

    //fclose(pFile);
    wstring strFilePath = _FilePath;

    if (strFilePath == L"C:\\Users\\82103\\Desktop\\DirectxPortfolio_420\\DirectX53\\OutputFile\\Content\\Flipbook\\Zmora_Spawn.flip")
    {
        int a = 0;
    }

    //strFilePath = CPathMgr::GetContentPath();
    //strFilePath += (L"Flipbook\\" + _FilePath);

    // Sprite 의 Key, RelativePath 를 저장
    FILE* File = nullptr;
    _wfopen_s(&File, strFilePath.c_str(), L"r");

    wchar_t Buff[255] = {};

    while (true)
    {
        if (EOF == fwscanf_s(File, L"%s", Buff, 255))
            break;

        wstring strRead = Buff;

        if (strRead == L"[KEY]")
        {
            fwscanf_s(File, L"%s", Buff, 255);
            SetKey(Buff);
        }

        else if (strRead == L"[PATH]")
        {
            fwscanf_s(File, L"%s", Buff, 255);
            SetRelativePath(Buff);
        }

        else if (strRead == L"[SPRITE_COUNT]")
        {
            int SpriteCount = 0;
            fwscanf_s(File, L"%d", &SpriteCount);
            m_vecSprite.resize(SpriteCount);
        }

        else if (strRead == L"[INDEX]")
        {
            int index = 0;
            fwscanf_s(File, L"%d", &index);
            fwscanf_s(File, L"%s", Buff, 255);
            fwscanf_s(File, L"%s", Buff, 255);

            wstring SpriteKey = Buff;

            fwscanf_s(File, L"%s", Buff, 255);
            fwscanf_s(File, L"%s", Buff, 255);

            wstring SpritePath = Buff;

            m_vecSprite[index] = CAssetMgr::GetInst()->LoadSprite(SpriteKey, SpritePath);
        }
    }


    fclose(File);

    return 0;

    return S_OK;
}
