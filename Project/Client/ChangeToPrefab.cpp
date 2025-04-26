#include "pch.h"
#include "ChangeToPrefab.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CTaskMgr.h>

#include <Engine/CPrefab.h>

#include "Content.h"

ChangeToPrefab::ChangeToPrefab()
{
}

ChangeToPrefab::~ChangeToPrefab()
{
}

void ChangeToPrefab::Init()
{

}

void ChangeToPrefab::Update_UI()
{

}

void ChangeToPrefab::Render_UI()
{
    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::Button("ChangeToPrefab", ImVec2(150, 30));
    ImGui::PopStyleColor(3);
    ImGui::PopID();
    //MemoryLeak 메모리 누수
    vector<string> ItemList;

    //// 게임 오브젝트를 가져옴
    CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    assert(pLevel);

    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        CLayer* pLayer = pLevel->GetLayer(i);
        const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

        for (int j = 0; j < vecObj.size(); j++) {
            //objNameStr.c_str()는 std::string의 내부 버퍼를 반환하는데, std::vector에 추가되는 것이 아니라, 같은 메모리 주소를 가리키는 포인터를 추가하는 것이야.

            //objNameStr은 for문에서 계속 덮어쓰이므로, ItemList에는 모두 같은 주소가 저장됨.
            string objNameStr = ToString(vecObj[j]->GetName());  // wstring을 string으로 변환
            if (MakePrefab) {
                if (objNameStr.c_str() == PrefabSTR) {
                    //Prefab 생성후 AssetMgr에 등록
                    Ptr<CPrefab> pPrefab = new CPrefab;
                    pPrefab->SetGameObject(vecObj[j]);
                    CAssetMgr::GetInst()->AddAsset(L"Prefab\\" + ToWString(objNameStr), pPrefab.Get());
                    pPrefab->Save(pPrefab->GetKey());

                    //vecObj[j]->Destroy();

                    MakePrefab = false;
                }
            }
            if (DeleteObject) {
                if (objNameStr.c_str() == DeleteSTR) {

                    vecObj[j]->Destroy();
                    CTaskMgr::GetInst()->m_LevelChanged = true;
                    
                }
            }
            ItemList.push_back(objNameStr);

        }
    }

    vector<const char*> charPtrList;
   
    for (auto& str : ItemList) {
        charPtrList.push_back(str.c_str());
    }

	  // 선택된 인덱스

	ImGui::Combo("##ObjectList", &selectedIndex, charPtrList.data(), charPtrList.size());

    if (ImGui::Button("MakePreFab", ImVec2(100, 30))) {
        PrefabSTR = charPtrList[selectedIndex];
        MakePrefab = true;
    };

    ImGui::Text("\n");

    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
    ImGui::Button("DeleteObject", ImVec2(150, 30));
    ImGui::PopStyleColor(3);
    ImGui::PopID();

    ImGui::Combo("##DeleteObjectList", &DeleteObjectIndex, charPtrList.data(), charPtrList.size());

    if (ImGui::Button("DeleteObject", ImVec2(100, 30))) {
        DeleteSTR = charPtrList[DeleteObjectIndex];
        DeleteObject = true;
    };

}
