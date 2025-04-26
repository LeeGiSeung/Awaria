#include "pch.h"
#include "FlipBookEditor.h"

#include <commdlg.h>

#include <Engine/CAssetMgr.h>
#include <Engine/CFlipbook.h>
#include <Engine/CSprite.h>


#include <shlwapi.h>  // PathFindFileName ���
#pragma comment(lib, "Shlwapi.lib")  // ���̺귯�� ��ũ

static vector<char> OriginalName(256, '\0');
static vector<char> SpritelName(256, '\0');

float normalizea(float value, float min, float max) {
    if (max == min) {

        return 0.0f; // ���� ó��
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
    wstring folderPath = CPathMgr::GetContentPath();  // ���� ���
    //folderPath += L"Flipbook\\";
    wstring searchPath = folderPath + L"*.flip";  // Ư�� Ȯ���� (*.flip) �˻�

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        wstring fileName = findData.cFileName;  // ���� �̸� ��������

        // Ȯ���� ����
        size_t pos = fileName.find_last_of(L".");
        wstring NoExtensionFileName = (pos != wstring::npos) ? fileName.substr(0, pos) : fileName;

        // ���� �ε� �Լ� ȣ�� (Ȯ���� ���� ���ϸ�, ��ü ���ϸ� ����)
        CAssetMgr::GetInst()->LoadFlipbook(L"Flipbook\\" + fileName, L"Flipbook\\" + fileName);

    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void FlipBookEditor::LoadAllSpriteFiles()
{
    wstring folderPath = CPathMgr::GetContentPath();// ���� ���
    folderPath += L"Sprite\\";
    wstring searchPath = folderPath + L"*.sprite";  // Ư�� Ȯ���� (*.flip) �˻�

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
        wstring fileName = findData.cFileName;  // ���� �̸� ��������

        // Ȯ���� ����
        size_t pos = fileName.find_last_of(L".");
        wstring NoExtensionFileName = (pos != wstring::npos) ? fileName.substr(0, pos) : fileName;

        // ���� �ε� �Լ� ȣ�� (Ȯ���� ���� ���ϸ�, ��ü ���ϸ� ����)
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

        // ���� ��� ���ڿ�
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
            if (pos != wstring::npos)  // Ȯ���ڰ� �ִٸ�
            {
                NoextensionfileName = NoextensionfileName.substr(0, pos);  // Ȯ���� ����
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

    ImGui::Image((ImTextureID)mPtex->GetSRV().Get() // �������� �ؽ��� ShaderResourceView
        , ImVec2(200.f, 200.f)			   // �ؽ��� ���� ũ��
        , cur_uv_min, cur_uv_max				   // ����� �ؽ��� UV ����
        , tint_col, border_col);		   // ƾƮ, ���� �÷�

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

        //FlipBookIndex�� �ٲ�����
        // �ø����� ������ �� ���� �������� �ʱ�ȭ
        if (Cur_FlipbookListIndex != FlipbookListIndex) {
            // ���ο� �ø��� ��������
            Ptr<CFlipbook> CurFlipbook = CAssetMgr::GetInst()->FindAsset<CFlipbook>(
                wstring(vecNames[FlipbookListIndex].begin(), vecNames[FlipbookListIndex].end())
            );

            // �������� �ʱ�ȭ (���ο� �ø��Ͽ��� ���� �������� ���������� �� ��)
            m_SelectIndex = 0;

            // �� �ø����� ù ��° ��������Ʈ ��������
            if (CurFlipbook->GetSpriteCount() > 0) {
                CurSprite = CurFlipbook->GetSprite(0);
                CurSpriteTex = CurSprite->GetAtlasTex();
                pTex = CurSpriteTex;

                // ���� ��������Ʈ�� �⺻ ������ �� ����
                CurOffset = CurSprite->GetOffset();
                //m_Offset_x = static_cast<int>(CurOffset.x * pTex->GetWidth());
                //m_Offset_y = static_cast<int>(CurOffset.y * pTex->GetHeight());      

                //m_Offset_x = CurOffset.x;
                //m_Offset_y = CurOffset.y;
            }

            // ���� �ø��� �ε��� ������Ʈ
            Cur_FlipbookListIndex = FlipbookListIndex;
        }

        m_Offset_x = CurSprite->GetIntOffset().x;
        m_Offset_y = CurSprite->GetIntOffset().y;

        //FlipBook������ SpriteIndex�� �ٲ�
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
        //�̰� �߰��� 04.03

        ImVec2 cur_uv_min = ImVec2(
            LeftTop_x + CurOffset.x, LeftTop_y + CurOffset.y
        );

        ImVec2 cur_uv_max = ImVec2(
            LeftTop_x + Slice_x + CurOffset.x, LeftTop_y + Slice_y + CurOffset.y
        );

        ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
        ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

        ImGui::Image((ImTextureID)pTex->GetSRV().Get() // �������� �ؽ��� ShaderResourceView
            , ImVec2(300.f, 300.f)			   // �ؽ��� ���� ũ��
            , cur_uv_min, cur_uv_max				   // ����� �ؽ��� UV ����
            , tint_col, border_col);		   // ƾƮ, ���� �÷�

        // �̹����� ȭ�� �� ��ġ�� ũ�� ��������
        ImVec2 img_min = ImGui::GetItemRectMin(); // �̹����� ���� �� ��ǥ
        ImVec2 img_max = ImGui::GetItemRectMax(); // �̹����� ������ �Ʒ� ��ǥ
        ImVec2 center = ImVec2((img_min.x + img_max.x) * 0.5f, (img_min.y + img_max.y) * 0.5f);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImU32 red = IM_COL32(255, 0, 0, 255);
        float line_thickness = 2.0f;
        // ���μ�
        draw_list->AddLine(ImVec2(img_min.x, center.y), ImVec2(img_max.x, center.y), red, line_thickness);
        // ���μ�
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


