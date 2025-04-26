#include "pch.h"
#include "Menu.h"

#include <commdlg.h>

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

#include <Engine/CTaskMgr.h>
#include <Lecture/CScriptMgr.h>

#include "ImGuiMgr.h"
#include "Inspector.h"
#include "SpriteEditor.h"
#include "FlipBookEditor.h"
#include "TileMapEditor.h"
#include "ObjectEditor.h"
#include "CollisionManagerUI.h"
#include "CutSceneEditor.h"
#include "ChangeToPrefab.h"

#include "CLevelSaveLoad.h"



Menu::Menu()
    : m_SpriteEditor(false)
    , m_FlipbookEditor(false)
    , m_CloneLevel(nullptr)
{
}

Menu::~Menu()
{
    if (nullptr != m_CloneLevel)
    {
        delete m_CloneLevel;
    }
}


void Menu::Render()
{
	if (!IsActive())
		return;
     
    if (ImGui::BeginMainMenuBar())
    {
        Render_UI();

        ImGui::EndMainMenuBar();
    }
}

void Menu::Update_UI()
{
}

void Menu::Render_UI()
{
    File();

    Level();

    Editor();

    Script();

    Asset();
}

void Menu::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Level Save"))
        {
            wstring strContentPath = CPathMgr::GetContentPath();
            strContentPath += L"Level";

            // 파일 경로 문자열
            wchar_t szFilePath[255] = {};

            OPENFILENAME Desc = {};

            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = nullptr;
            Desc.lpstrFile = szFilePath;	// 최종적으로 고른 경로를 받아낼 목적지
            Desc.nMaxFile = 255;
            Desc.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
            Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            Desc.lpstrInitialDir = strContentPath.c_str();

            if (GetSaveFileName(&Desc))
            {
                CLevelSaveLoad::SaveLevel(szFilePath);                
            }            

            wstring strBinPath = CPathMgr::GetBinPath();
            SetCurrentDirectory(strBinPath.c_str());
        }

        if (ImGui::MenuItem("Level Load"))
        {
            wstring strContentPath = CPathMgr::GetContentPath();
            strContentPath += L"Level";

            // 파일 경로 문자열
            wchar_t szFilePath[255] = {};

            OPENFILENAME Desc = {};

            Desc.lStructSize = sizeof(OPENFILENAME);
            Desc.hwndOwner = nullptr;
            Desc.lpstrFile = szFilePath;
            Desc.nMaxFile = 255;
            Desc.lpstrFilter = L"Level\0*.lv\0ALL\0*.*";
            Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            Desc.lpstrInitialDir = strContentPath.c_str();

            if (GetOpenFileName(&Desc))
            {
                CLevel* pNextLevel = CLevelSaveLoad::LoadLevel(szFilePath);

                tTask task = {};
                task.Type = TASK_TYPE::CHANGE_LEVEL;
                task.Param0 = (DWORD_PTR)pNextLevel;

                CTaskMgr::GetInst()->AddTask(task);
            }            

            wstring strBinPath = CPathMgr::GetBinPath();
            SetCurrentDirectory(strBinPath.c_str());
        }

        ImGui::EndMenu();
    }
}

void Menu::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();        
        LEVEL_STATE State = pCurLevel->GetState();

        bool IsPlay = State == LEVEL_STATE::PLAY;
        bool IsPause = State == LEVEL_STATE::PAUSE;
        bool IsStop = State == LEVEL_STATE::STOP;

        tTask task = {};
        task.Type = TASK_TYPE::CHANGE_LEVEL_STATE;

        if (ImGui::MenuItem("Play", nullptr, &IsPlay, IsPause || IsStop))
        {
            // 현재 레벨을 복사해둠
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            m_CloneLevel = pCurLevel->Clone();

            task.Param0 = (DWORD_PTR)LEVEL_STATE::PLAY;
            CTaskMgr::GetInst()->AddTask(task);
        }

        if (ImGui::MenuItem("Pause", nullptr, &IsPause, IsPlay))
        {
            task.Param0 = (DWORD_PTR)LEVEL_STATE::PAUSE;
            CTaskMgr::GetInst()->AddTask(task);
        }

        if (ImGui::MenuItem("Stop", nullptr, &IsStop, IsPlay || IsPause))
        {
            // Play 버튼을 눌렀을 당시 상태를 복제해놨던 레벨로 다시 변경한다.
            tTask LevelChanetask = {};
            LevelChanetask.Type = TASK_TYPE::CHANGE_LEVEL;
            LevelChanetask.Param0 = (DWORD_PTR)m_CloneLevel;
            m_CloneLevel = nullptr;
            CTaskMgr::GetInst()->AddTask(LevelChanetask);

            // 변경한 레벨을 Stop 상태로 만든다.            
            task.Param0 = (DWORD_PTR)LEVEL_STATE::STOP;
            CTaskMgr::GetInst()->AddTask(task);

            // Inspector 가 보여주는 오브젝트를 날린다.
            Inspector* pInspector = ImGuiMgr::GetInst()->FindUI<Inspector>("Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

void Menu::Editor()
{
    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("Sprite Editor", nullptr, &m_SpriteEditor))
        {
            SpriteEditor* pSpriteEditor = ImGuiMgr::GetInst()->FindUI<SpriteEditor>("SpriteEditor");
            if (m_SpriteEditor)
            {
                pSpriteEditor->SetActive(true);
            }
            else
            {
                pSpriteEditor->SetActive(false);
            }
        }

        if (ImGui::MenuItem("Flipbook Editor", nullptr, &m_FlipbookEditor))
        {
            FlipBookEditor* pFlipBookEditor = ImGuiMgr::GetInst()->FindUI<FlipBookEditor>("FlipBookEditor");
            if (m_FlipbookEditor)
            {
                pFlipBookEditor->SetActive(true);
            }
            else
            {
                pFlipBookEditor->SetActive(false);
            }
        }

        if (ImGui::MenuItem("TileMap Editor", nullptr, &m_TileEditor))
        {
            TileMapEditor* pTileMapEditor = ImGuiMgr::GetInst()->FindUI<TileMapEditor>("TileMapEditor");
            if (m_TileEditor)
            {
                pTileMapEditor->SetActive(true);
            }
            else
            {
                pTileMapEditor->SetActive(false);
            }
        }

        if (ImGui::MenuItem("Object Editor", nullptr, &m_ObjectEditor))
        {
            ObjectEditor* pObjectEditor = ImGuiMgr::GetInst()->FindUI<ObjectEditor>("ObjectEditor");
            if (m_ObjectEditor)
            {
                pObjectEditor->SetActive(true);
            }
            else
            {
                pObjectEditor->SetActive(false);
            }
        }
        if (ImGui::MenuItem("CollisionManager UI", nullptr, &m_CollisionManagerUI))
        {
            CollisionManagerUI* pObjectEditor = ImGuiMgr::GetInst()->FindUI<CollisionManagerUI>("CollisionManagerUI");
            if (m_CollisionManagerUI)
            {
                pObjectEditor->SetActive(true);
            }
            else
            {
                pObjectEditor->SetActive(false);
            }
        }

        if (ImGui::MenuItem("CutScene Editor", nullptr, &m_CutSceneEditor))
        {
            CutSceneEditor* pObjectEditor = ImGuiMgr::GetInst()->FindUI<CutSceneEditor>("CutSceneEditor");
            if (m_CutSceneEditor)
            {
                pObjectEditor->SetActive(true);
            }
            else
            {
                pObjectEditor->SetActive(false);
            }
        }

        if (ImGui::MenuItem("Prefab Editor", nullptr, &m_Prefab))
        {
            ChangeToPrefab* pObjectEditor = ImGuiMgr::GetInst()->FindUI<ChangeToPrefab>("ChangeToPrefab");
            if (m_Prefab)
            {
                pObjectEditor->SetActive(true);
            }
            else
            {
                pObjectEditor->SetActive(false);
            }
        }

        ImGui::EndMenu();
    }
}

void Menu::Script()
{
    if (ImGui::BeginMenu("Scripts"))
    {
        vector<wstring> vecScriptName;
        CScriptMgr::GetScriptInfo(vecScriptName);

        for (int i = 0; i < vecScriptName.size(); ++i)
        {
            if (ImGui::MenuItem(ToString(vecScriptName[i]).c_str()))
            {
                Inspector* pInspector = ImGuiMgr::GetInst()->FindUI<Inspector>("Inspector");

                CGameObject* pObject = pInspector->GetTargetObject();

                if (nullptr != pObject)
                {
                    pObject->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                    pInspector->SetTargetObject(pObject);
                }
            }
        }    

        ImGui::EndMenu();
    }
}

void Menu::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Material"))
        {
            Ptr<CMaterial> pMtrl = new CMaterial;
            string MtrlName = GetMaterialName();
            CAssetMgr::GetInst()->AddAsset(ToWString(MtrlName), pMtrl.Get());

            wstring MtrlPath = CPathMgr::GetContentPath();
            MtrlPath = MtrlPath + L"Material\\" + ToWString(MtrlName) + L".mtrl";
            pMtrl->Save(MtrlPath);
        }

        ImGui::EndMenu();
    }
}

string Menu::GetMaterialName()
{
    char szMtrlName[255] = {};

    int i = 0;
    while (true)
    {
        sprintf_s(szMtrlName, 255, "Default Material %d", i++);
        
        Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(ToWString(szMtrlName));

        if (nullptr == pMtrl)
            break;
    }

    return szMtrlName;
}
