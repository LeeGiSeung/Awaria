#include "pch.h"
#include "ImGuiMgr.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "EditorUI.h"

ImGuiMgr::ImGuiMgr()
    : m_ShowDemo(true)
{

}

ImGuiMgr::~ImGuiMgr()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // 모든 UI 삭제
    /*map<string, EditorUI*>::iterator iter = m_mapUI.begin();
    for (; iter != m_mapUI.end(); ++iter)
    {
        delete iter->second;
    }*/
    for (const auto& pair : m_mapUI)
    {
        delete pair.second;
    }

    m_mapUI.clear();
}



void ImGuiMgr::Progress()
{
    // ImGui 시작
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Demo 기능 소개용 UI
    if (m_ShowDemo)
    {
        ImGui::ShowDemoWindow();
    }

    // UI 업데이트             
    for (const auto& pair : m_mapUI)
    {
        pair.second->Update();
    }

    // UI 렌더링
    for (const auto& pair : m_mapUI)
    {
        pair.second->Render();
    }
            
    // ImGui Rendering
    // 메인 윈도우 안쪽에 있는 UI 렌더링
    ImGui::Render();    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // 메인 윈도우 밖에 있는 별개의 윈도우에 그려지는 UI 렌더링
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}