#include "pch.h"
#include "CollisionManagerUI.h"

#include <Engine/CCollisionMgr.h>

CollisionManagerUI::CollisionManagerUI()
{
}

CollisionManagerUI::~CollisionManagerUI()
{
}

void CollisionManagerUI::Init()
{
}

void CollisionManagerUI::Update_UI()
{
}

void CollisionManagerUI::Render_UI()
{
    const int layerCount = static_cast<int>(COLLISION_LAYER::END);

    static bool collisionMatrix[MAX_LAYER][MAX_LAYER] = { false };

    if (ImGui::BeginTable("CollisionMatrix", layerCount + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchSame))
    {
        // ù ��° �� (X�� ���̾� �̸�)
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); // ��ĭ
        for (int x = 0; x < layerCount; ++x)
        {
            ImGui::TableNextColumn();
            ImGui::Text(COLLISION_LAYER_STR[(UINT)x], x);
        }

        // ������ �� (Y�� ���̾� �̸� + üũ�ڽ�)
        for (int y = 0; y < layerCount; ++y)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(COLLISION_LAYER_STR[(UINT)y], y);

            for (int x = 0; x < layerCount; ++x)
            {
                ImGui::TableNextColumn();

                // ��� ������ ���� ���� �� ũ�⸦ ������
                float cellWidth = ImGui::GetColumnWidth();
                float checkboxWidth = ImGui::CalcTextSize("?").x; // üũ�ڽ� ũ�� ����
                float padding = (cellWidth - checkboxWidth) * 0.5f;

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);

                std::string label = "##chk" + std::to_string(x) + "_" + std::to_string(y);
                if (ImGui::Checkbox(label.c_str(), &collisionMatrix[x][y]))
                {
                    CCollisionMgr::GetInst()->CollisionLayerCheck(x, y);
                }
            }
        }

        ImGui::EndTable();
    }
}
