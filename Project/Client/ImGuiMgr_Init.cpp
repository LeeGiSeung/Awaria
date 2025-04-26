#include "pch.h"
#include "ImGuiMgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "EditorUI.h"

void ImGuiMgr::Init(HWND _hMainWind, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _Context)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // ImGui 에 MainWindow 전달
    ImGui_ImplWin32_Init(_hMainWind);

    // ImGui 에 Deviec, Context 를 알림
    ImGui_ImplDX11_Init(_Device.Get(), _Context.Get());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Editor 용 UI 생성
    CreateEditorUI();
}


#include "Menu.h"
#include "Inspector.h"
#include "Outliner.h"
#include "Content.h"
#include "ListUI.h"
#include "FlipBookEditor.h"
#include "SpriteEditor.h"
#include "TileMapEditor.h"
#include "ObjectEditor.h"
#include "CollisionManagerUI.h"
#include "CutSceneEditor.h"
#include "ChangeToPrefab.h"

void ImGuiMgr::CreateEditorUI()
{
    EditorUI* pUI = nullptr;

    pUI = new Menu;
    pUI->SetName("Menu");
    pUI->SetActive(true);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new Inspector;
    pUI->SetName("Inspector");
    pUI->SetActive(true);     
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new Outliner;
    pUI->SetName("Outliner");
    pUI->SetActive(true);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new Content;
    pUI->SetName("Content");
    pUI->SetActive(true);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ListUI;
    pUI->SetName("ListUI");
    pUI->SetActive(false);
    pUI->SetModal(true);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new SpriteEditor;
    pUI->SetName("SpriteEditor");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new FlipBookEditor;
    pUI->SetName("FlipBookEditor");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new TileMapEditor;
    pUI->SetName("TileMapEditor");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ObjectEditor;
    pUI->SetName("ObjectEditor");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new CollisionManagerUI;
    pUI->SetName("CollisionManagerUI");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new CutSceneEditor;
    pUI->SetName("CutSceneEditor");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));

    pUI = new ChangeToPrefab;
    pUI->SetName("ChangeToPrefab");
    pUI->SetActive(false);
    pUI->Init();
    m_mapUI.insert(make_pair(pUI->GetName(), pUI));
}
