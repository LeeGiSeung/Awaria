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

    // ��� UI ����
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
    // ImGui ����
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Demo ��� �Ұ��� UI
    if (m_ShowDemo)
    {
        ImGui::ShowDemoWindow();
    }

    // UI ������Ʈ             
    for (const auto& pair : m_mapUI)
    {
        pair.second->Update();
    }

    // UI ������
    for (const auto& pair : m_mapUI)
    {
        pair.second->Render();
    }
            
    // ImGui Rendering
    // ���� ������ ���ʿ� �ִ� UI ������
    ImGui::Render();    
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // ���� ������ �ۿ� �ִ� ������ �����쿡 �׷����� UI ������
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}