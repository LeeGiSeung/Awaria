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
        // 첫 번째 행 (X축 레이어 이름)
        ImGui::TableNextRow();
        ImGui::TableNextColumn(); // 빈칸
        for (int x = 0; x < layerCount; ++x)
        {
            ImGui::TableNextColumn();
            ImGui::Text(COLLISION_LAYER_STR[(UINT)x], x);
        }

        // 나머지 행 (Y축 레이어 이름 + 체크박스)
        for (int y = 0; y < layerCount; ++y)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(COLLISION_LAYER_STR[(UINT)y], y);

            for (int x = 0; x < layerCount; ++x)
            {
                ImGui::TableNextColumn();

                // 가운데 정렬을 위해 현재 셀 크기를 가져옴
                float cellWidth = ImGui::GetColumnWidth();
                float checkboxWidth = ImGui::CalcTextSize("?").x; // 체크박스 크기 추정
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
