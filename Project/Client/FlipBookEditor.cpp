#include "pch.h"
#include "FlipBookEditor.h"

#include <commdlg.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CFlipbook.h>
#include <Engine/CSprite.h>


#include <shlwapi.h>  // PathFindFileName 사용
#pragma comment(lib, "Shlwapi.lib")  // 라이브러리 링크

static vector<char> OriginalName(256, '\0');
static vector<char> SpritelName(256, '\0');

float normalizea(float value, float min, float max) {
    if (max == min) {

        return 0.0f; // 오류 처리
    }
    return (value - min) / (max - min);
}

FlipBookEditor::FlipBookEditor() : m_CurFlipbook(nullptr)
{
    FlipbookListIndex = -1;
    FlipbookListIndex0 = -1;
    Cur_FlipbookListIndex = 0;
    SpriteListIndex = -1;

    m_SelectIndex = 0;
    m_PosetSelectIndex = 0;
    LeftTop_x = 0.f;
    LeftTop_y = 0.f;
    Slice_x = 0.f;
    Slice_y = 0.f;
}

FlipBookEditor::~FlipBookEditor()
{
}


void FlipBookEditor::LoadAllFlipbookFiles()
{
    wstring folderPath = CPathMgr::GetContentPath();  // 폴더 경로
    //folderPath += L"Flipbook\\";
    wstring searchPath = folderPath + L"*.flip";  // 특정 확장자 (*.flip) 검색

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        wstring fileName = findData.cFileName;  // 파일 이름 가져오기

        // 확장자 제거
        size_t pos = fileName.find_last_of(L".");
        wstring NoExtensionFileName = (pos != wstring::npos) ? fileName.substr(0, pos) : fileName;

        // 파일 로드 함수 호출 (확장자 없는 파일명, 전체 파일명 전달)
        CAssetMgr::GetInst()->LoadFlipbook(L"Flipbook\\" + fileName, L"Flipbook\\" + fileName);

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void FlipBookEditor::LoadAllSpriteFiles()
{
    wstring folderPath = CPathMgr::GetContentPath();// 폴더 경로
    folderPath += L"Sprite\\";
    wstring searchPath = folderPath + L"*.sprite";  // 특정 확장자 (*.flip) 검색

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        wstring fileName = findData.cFileName;  // 파일 이름 가져오기

        // 확장자 제거
        size_t pos = fileName.find_last_of(L".");
        wstring NoExtensionFileName = (pos != wstring::npos) ? fileName.substr(0, pos) : fileName;

        // 파일 로드 함수 호출 (확장자 없는 파일명, 전체 파일명 전달)
        CAssetMgr::GetInst()->LoadSprite(L"Sprite\\" + fileName, L"Sprite\\" + fileName);

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void FlipBookEditor::Init()
{

}

void FlipBookEditor::Update_UI()
{
}

void FlipBookEditor::Render_UI()
{

    /*#######################
            MakeFlipbook
    #######################*/

    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::Button("Make_FlipBook", ImVec2(150, 30));
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    ImGui::Text("\n");

    if (SpriteListIndex == -1) {
        SpriteListIndex = 0;
    }

    if (FlipbookListIndex0 == -1) {
        FlipbookListIndex0 = 0;
    }

    if (Cur_FlipbookListIndex == -1) {
        Cur_FlipbookListIndex = 0;
    }


    ImGui::Text("FlipBookName");
    ImGui::InputText("##FlipBookName", OriginalName.data(), OriginalName.size() + 50);
    if (ImGui::Button("AddFlipbook Asset", ImVec2(150, 20))) {
        Ptr<CFlipbook> pFlipbook = new CFlipbook;

        string FlipbookNamestr = string(OriginalName.data(), strnlen(OriginalName.data(), OriginalName.size()));

        FlipbookNamestr = "Flipbook\\" + FlipbookNamestr;

        wchar_t FlipbookName[255] = {};
        swprintf_s(FlipbookName, 255, L"%hs", FlipbookNamestr.c_str());

        //CAssetMgr::GetInst()->AddAsset(FlipbookName, pFlipbook.Get());

        pFlipbook->Save(FlipbookName);
    }

    /*#######################
        FlipBookAddSprite
    #######################*/

    if (FlipbookListIndex == -1) {
        FlipbookListIndex = 0;
    }

    ImGui::Text("\n");
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::Button("FlipBookAddSprite", ImVec2(150, 30));
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    ImGui::Text("\n");

    if (ImGui::Button("FlipBookFileLoad", ImVec2(150, 30))) {
        wstring strContentPath = CPathMgr::GetContentPath();
        strContentPath += L"Flipbook";

        // 파일 경로 문자열
        wchar_t szFilePath[255] = {};

        OPENFILENAME Desc = {};

        Desc.lStructSize = sizeof(OPENFILENAME);
        Desc.hwndOwner = nullptr;
        Desc.lpstrFile = szFilePath;
        Desc.nMaxFile = 255;
        Desc.lpstrFilter = L"Flipbook\0*.flip\0ALL\0*.*";
        Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        Desc.lpstrInitialDir = strContentPath.c_str();

        if (GetOpenFileName(&Desc))
        {

            wstring fileName = PathFindFileName(szFilePath);
            wstring NoextensionfileName = PathFindFileName(szFilePath);
            size_t pos = NoextensionfileName.find_last_of(L".");
            if (pos != wstring::npos)  // 확장자가 있다면
            {
                NoextensionfileName = NoextensionfileName.substr(0, pos);  // 확장자 제거
            }

            CAssetMgr::GetInst()->LoadFlipbook(NoextensionfileName, fileName);
        }

        wstring strBinPath = CPathMgr::GetBinPath();
        SetCurrentDirectory(strBinPath.c_str());
    }

    ImGui::Text("FlipBookList");
    static vector<string> vecFlipbookNames;
    vecFlipbookNames.clear();
    CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)FLIPBOOK, vecFlipbookNames);
    Ptr<CFlipbook> pFlipbook2 = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(vecFlipbookNames[FlipbookListIndex0].begin(), vecFlipbookNames[FlipbookListIndex0].end()));
    vector<const char*> itemsFlip;
    for (const auto& name : vecFlipbookNames) {
        itemsFlip.push_back(name.c_str());
    }
    ImGui::Combo("##AddFlipbookSprite", &FlipbookListIndex0, itemsFlip.data(), itemsFlip.size());

    static vector<string> vecSpriteName;
    vecSpriteName.clear();
    CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)SPRITE, vecSpriteName);
    Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(wstring(vecSpriteName[SpriteListIndex].begin(), vecSpriteName[SpriteListIndex].end()));
    vector<const char*> itemsSprite;

    for (const auto& name : vecSpriteName) {
        itemsSprite.push_back(name.c_str());
    }

    LoadAllSpriteFiles();
    LoadAllFlipbookFiles();

    ImGui::Text("SpriteList");
    ImGui::Combo("##SpriteList", &SpriteListIndex, itemsSprite.data(), itemsSprite.size());
    ImGui::Text("\n");

    Ptr<CTexture> mPtex = pSprite->GetAtlasTex();
    LeftTop_x = pSprite->GetLeftTop().x;
    LeftTop_y = pSprite->GetLeftTop().y;
    Slice_x = pSprite->GetSlice().x;
    Slice_y = pSprite->GetSlice().y;

    ImGui::Text("AtlasName : ");
    ImGui::SameLine(100);
    ImGui::Text(ToString(pSprite->GetAtlasTex()->GetKey()).c_str());

    ImVec2 cur_uv_min = ImVec2(
        LeftTop_x, LeftTop_y
    );

    ImVec2 cur_uv_max = ImVec2(
        LeftTop_x + Slice_x, LeftTop_y + Slice_y
    );

    ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
    ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

    ImGui::Image((ImTextureID)mPtex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
        , ImVec2(200.f, 200.f)			   // 텍스쳐 위젯 크기
        , cur_uv_min, cur_uv_max				   // 출력할 텍스쳐 UV 범위
        , tint_col, border_col);		   // 틴트, 보더 컬러

    ImGui::SameLine(300);
    if (ImGui::Button("AddSprite", ImVec2(100, 50))) {
        pFlipbook2->AddSprite(CAssetMgr::GetInst()->FindAsset<CSprite>(pSprite->GetKey()));
        pFlipbook2->Save(pFlipbook2->GetKey());
    };

    ImGui::Text("\n");

    if (ImGui::Button("FlipBookUpdate", ImVec2(150, 20))) {

        //string FlipbookNamestr = string(OriginalName.data(), strnlen(OriginalName.data(), OriginalName.size()));

        //wchar_t FlipbookName[255] = {};
        //swprintf_s(FlipbookName, 255, L"%hs", FlipbookNamestr.c_str());

        //pFlipbook2->Save(FlipbookName);
        pFlipbook2->Save(pFlipbook2->GetKey());
    }

    /*#######################
        SetOffset
    #######################*/

    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::Button("Offset_Setting", ImVec2(150, 30));
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    ImGui::Text("\n");
    int tab = 100;

    ImGui::Text("FlipBookList");
    static vector<string> vecNames;
    vecNames.clear();

    CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)FLIPBOOK, vecNames);

    Ptr<CFlipbook> pFlipbook = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(vecNames[FlipbookListIndex].begin(), vecNames[FlipbookListIndex].end()));

    vector<const char*> items;
    for (const auto& name : vecNames) {
        items.push_back(name.c_str());
    }

    ImGui::Combo("##Projection", &FlipbookListIndex, items.data(), items.size());

    ImGui::Text("\n");

    // Dummy
    ImVec2 button_sz(20, 20);

    ImGuiStyle& style = ImGui::GetStyle();
    int buttons_count = pFlipbook->GetSpriteCount();
    if (buttons_count > 0) {
        ImGui::Text("FlipBook Index");
        float window_visible_x2 = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
        for (int n = 0; n < buttons_count; n++)
        {
            string n_char = to_string(n);
            const char* cstr = n_char.c_str();

            ImGui::PushID(n);
            if (ImGui::Button(cstr, button_sz)) {
                m_SelectIndex = n;
            };
            if (ImGui::IsItemFocused()) {
                m_SelectIndex = n;
            }
            float last_button_x2 = ImGui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                ImGui::SameLine();
            ImGui::PopID();
        }

        Ptr<CSprite> CurSprite = pFlipbook->GetSprite(m_SelectIndex);
        Ptr<CTexture> CurSpriteTex = CurSprite->GetAtlasTex();
        Ptr<CTexture> pTex = CurSpriteTex;

        Vec2 CurOffset;

        ImGui::Text("\n");
        ImGui::Text("AtlasName : ");
        ImGui::SameLine(100);
        ImGui::Text(ToString(CurSprite->GetAtlasTex()->GetKey()).c_str());
        ImGui::Text("SpriteName : ");
        ImGui::SameLine(100);
        ImGui::Text(ToString(CurSprite->GetKey()).c_str());

        //FlipBookIndex가 바꼈을때
        // 플립북을 변경할 때 기존 오프셋을 초기화
        if (Cur_FlipbookListIndex != FlipbookListIndex) {
            // 새로운 플립북 가져오기
            Ptr<CFlipbook> CurFlipbook = CAssetMgr::GetInst()->FindAsset<CFlipbook>(
                wstring(vecNames[FlipbookListIndex].begin(), vecNames[FlipbookListIndex].end())
            );

            // 오프셋을 초기화 (새로운 플립북에는 이전 오프셋이 남아있으면 안 됨)
            m_SelectIndex = 0;

            // 새 플립북의 첫 번째 스프라이트 가져오기
            if (CurFlipbook->GetSpriteCount() > 0) {
                CurSprite = CurFlipbook->GetSprite(0);
                CurSpriteTex = CurSprite->GetAtlasTex();
                pTex = CurSpriteTex;

                // 현재 스프라이트의 기본 오프셋 값 적용
                CurOffset = CurSprite->GetOffset();
                //m_Offset_x = static_cast<int>(CurOffset.x * pTex->GetWidth());
                //m_Offset_y = static_cast<int>(CurOffset.y * pTex->GetHeight());      

                //m_Offset_x = CurOffset.x;
                //m_Offset_y = CurOffset.y;
            }

            // 현재 플립북 인덱스 업데이트
            Cur_FlipbookListIndex = FlipbookListIndex;
        }

        m_Offset_x = CurSprite->GetIntOffset().x;
        m_Offset_y = CurSprite->GetIntOffset().y;

        //FlipBook내에서 SpriteIndex가 바뀔때
        if (m_PosetSelectIndex != m_SelectIndex) {

            CurSprite = pFlipbook->GetSprite(m_SelectIndex);
            CurSpriteTex = CurSprite->GetAtlasTex();
            pTex = CurSpriteTex;

            float x = CurSprite->GetOffset().x;
            float y = CurSprite->GetOffset().y;
            //m_Offset_x = static_cast<int>(x * pTex->GetWidth());
            //m_Offset_y = static_cast<int>(y * pTex->GetHeight());
        }
        else {
            CurOffset = CurSprite->GetOffset();
        }

        LeftTop_x = CurSprite->GetLeftTop().x;
        LeftTop_y = CurSprite->GetLeftTop().y;
        Slice_x = CurSprite->GetSlice().x;
        Slice_y = CurSprite->GetSlice().y;
        //이거 추가함 04.03

        ImVec2 cur_uv_min = ImVec2(
            LeftTop_x + CurOffset.x, LeftTop_y + CurOffset.y
        );

        ImVec2 cur_uv_max = ImVec2(
            LeftTop_x + Slice_x + CurOffset.x, LeftTop_y + Slice_y + CurOffset.y
        );

        ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
        ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

        ImGui::Image((ImTextureID)pTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
            , ImVec2(300.f, 300.f)			   // 텍스쳐 위젯 크기
            , cur_uv_min, cur_uv_max				   // 출력할 텍스쳐 UV 범위
            , tint_col, border_col);		   // 틴트, 보더 컬러

        // 이미지의 화면 상 위치와 크기 가져오기
        ImVec2 img_min = ImGui::GetItemRectMin(); // 이미지의 왼쪽 위 좌표
        ImVec2 img_max = ImGui::GetItemRectMax(); // 이미지의 오른쪽 아래 좌표
        ImVec2 center = ImVec2((img_min.x + img_max.x) * 0.5f, (img_min.y + img_max.y) * 0.5f);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImU32 red = IM_COL32(255, 0, 0, 255);
        float line_thickness = 2.0f;
        // 가로선
        draw_list->AddLine(ImVec2(img_min.x, center.y), ImVec2(img_max.x, center.y), red, line_thickness);
        // 세로선
        draw_list->AddLine(ImVec2(center.x, img_min.y), ImVec2(center.x, img_max.y), red, line_thickness);

        ImGui::Text("Offset_x");
        ImGui::SameLine(100);
        if (ImGui::InputInt("##m_Offset_x", &m_Offset_x)) {
            CurSprite->SetOffset(Vec2(m_Offset_x, m_Offset_y));
        };


        ImGui::Text("Offset_y");
        ImGui::SameLine(100);
        if (ImGui::InputInt("##m_Offset_y", &m_Offset_y)) {
            CurSprite->SetOffset(Vec2(m_Offset_x, m_Offset_y));
        };

        if (m_PosetSelectIndex == m_SelectIndex && Cur_FlipbookListIndex == FlipbookListIndex) {
            CurSprite->SetOffset(Vec2(m_Offset_x, m_Offset_y));
        }

        if (ImGui::Button("SpriteUpdate", ImVec2(150, 20))) {
            CurSprite->Save(CurSprite->GetKey());
        }


        m_PosetSelectIndex = m_SelectIndex;
        Cur_FlipbookListIndex = FlipbookListIndex;
    }
}


