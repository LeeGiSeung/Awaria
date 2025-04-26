#include "pch.h"
#include "ObjectEditor.h"
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CAssetMgr.h>

#include <Engine/CScript.h>

//static vector<char> ObjectName(100, '\0');
//static vector<char> PrefabObjectName(256, '\0');
//static vector<char> MtrlName(256, '\0');

static char ObjectName[64] = "";
static char PrefabObjectName[64] = "";
static char MtrlName[64] = "";

//InputText("##DebugTextEncodingBuf", buf, IM_ARRAYSIZE(buf));

ObjectEditor::ObjectEditor()
{
}

ObjectEditor::~ObjectEditor()
{
}

void ObjectEditor::Init()
{
	componentFlags = { false };
}

void ObjectEditor::Update_UI()
{
}

void ObjectEditor::Render_UI()
{

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::Button("SpawnObject", ImVec2(150, 30));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::Text("\n");
	ImGui::Text("ObjectName");
	ImGui::SameLine(100);
	ImGui::PushItemWidth(150);
	//ImGui::InputText("##ObjectName", ObjectName.data(), ObjectName.size() + 50);
	ImGui::InputText("##ObjectName", ObjectName, IM_ARRAYSIZE(ObjectName));
	ImGui::PopItemWidth();

	ImGui::Text("\n");
	ImGui::PushItemWidth(150);
    if (ImGui::CollapsingHeader("ComponentCheck"))
	ImGui::PopItemWidth();
    {
        if (ImGui::BeginTable("split", 3))
        {
			for (int i = 0; i < COMPONENT_TYPE::COMPONENT_END; i++) {
				ImGui::TableNextColumn(); ImGui::Checkbox(COMPONENT_TYPE_STR[(UINT)i], &componentFlags[i]);
			}
			ImGui::EndTable(); // 테이블 닫기
        }
    }
	ImGui::Text("\n");
	if (componentFlags[FLIPBOOKRENDER]) {
		ImGui::Text("FlipBookList");
		ImGui::SameLine(100);
		static vector<string> vecFlipbookNames;
		vecFlipbookNames.clear();

		CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)FLIPBOOK, vecFlipbookNames);

		pFlipbook = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(vecFlipbookNames[FlipbookListIndex].begin(), vecFlipbookNames[FlipbookListIndex].end()));

		vector<const char*> itemsFlip;

		for (const auto& name : vecFlipbookNames) {
			itemsFlip.push_back(name.c_str());
		}

		ImGui::Combo("##AddFlipbookSpriteList", &FlipbookListIndex, itemsFlip.data(), itemsFlip.size());

		ImGui::Text("PlaySpeed");
		ImGui::SameLine(100);
		ImGui::InputInt("##PlaySpeed", &PlaySpeed);

		ImGui::Text("Repeat");
		ImGui::SameLine(210);
		ImGui::Checkbox("##Repeat", &Repeat);

		if (ImGui::Button("AddFlipBook", ImVec2(90, 30))) {
			FlipBookVector.push_back(pFlipbook);
		}
	}

	ImGui::Text("\n");
	ImGui::Text("\n");
	ImGui::Text("LayerIndex");
	ImGui::SameLine(100);
	ImGui::InputInt("##LayerInt", &LayerInt);

	//컴포넌트 추가하면 이거 수정해야함
	//Component
	if (ImGui::Button("SpawnObject", ImVec2(100, 30))) {
		CGameObject* pSpawnObj = new CGameObject;
		//wstring wOriginalName = wstring(ObjectName.begin(), ObjectName.end());
		wstring wOriginalName = ToWString(ObjectName);
		pSpawnObj->SetName(wOriginalName);
		for (int i = 0; i < COMPONENT_TYPE::COMPONENT_END; i++) {
			
			if (componentFlags[i]) {
				switch (i)
				{
					case TRANSFORM:
						pSpawnObj->AddComponent(new CTransform);

						pSpawnObj->Transform()->SetRelativePos(0.f, 0.f, 0.f);
						pSpawnObj->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

						break;
					case COLLIDER2D:
						pSpawnObj->AddComponent(new CCollider2D);

						pSpawnObj->Collider2D()->SetIndependentScale(true);
						pSpawnObj->Collider2D()->SetScale(Vec2(100.f, 100.f));
						pSpawnObj->Collider2D()->SetOffset(Vec2(0.f, 0.f));

						break;
					case COLLIDER3D:
						//pSpawnObj->AddComponent(new CCollider3D);
						break;
					case LIGHT2D:
						pSpawnObj->AddComponent(new CLight2D);

						pSpawnObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
						pSpawnObj->Light2D()->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
						pSpawnObj->Light2D()->SetRadius(300.f);

						break;
					case LIGHT3D:
						//pSpawnObj->AddComponent(new CTransform);
						break;
					case CAMERA:
						pSpawnObj->AddComponent(new CCamera);

						pSpawnObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
						pSpawnObj->Camera()->SetPriority(0); // 메인 카메라로 설정
						pSpawnObj->Camera()->CheckLayerAll();
						pSpawnObj->Camera()->CheckLayer(31);

						break;
					case BOUNDINGBOX:
						//pSpawnObj->AddComponent(new CTransform);
						break;
					case MESHRENDER:

						pSpawnObj->AddComponent(new CMeshRender);

						pSpawnObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
						pSpawnObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

						break;
					case SPRITERENDER:
						//pSpawnObj->AddComponent(new CSpriteRender);
						break;
					case FLIPBOOKRENDER:
						pSpawnObj->AddComponent(new CFlipbookRender);

						for (int i = 0; i < FlipBookVector.size(); i++) {
							pSpawnObj->FlipbookRender()->AddFlipbook(i, FlipBookVector[i]);
						}

						//0번 Flipbook 기본 재생 설정
						pSpawnObj->FlipbookRender()->Play(0, PlaySpeed, Repeat);

						break;
					case TILERENDER:
						//pSpawnObj->AddComponent(new CTileRender);
						break;
					case PARTICLERENDER:
						pSpawnObj->AddComponent(new CParticleRender);
						break;
					case SKYBOX:
						//pSpawnObj->AddComponent(new CTransform);
						break;
					case DECAL:
						//pSpawnObj->AddComponent(new CTransform);
						break;
					case LANDSCAPE:
						//pSpawnObj->AddComponent(new CTransform);
						break;
					case COMPONENT_END:
						//pSpawnObj->AddComponent(new CTransfor);
						break;
					case SCRIPT:
						//pSpawnObj->AddComponent(new CScript);
						break;
				default:
					break;
				}
			}
		}

		//CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		SpawnGameObject(pSpawnObj, LayerInt);

		//새로운 오브젝트 맞이하기 위해 벡터 클리어
		FlipBookVector.clear();
		//PlaySpeed도 초기화
		PlaySpeed = 0;
	};
	
	ShowCollisionTable();

	ImGui::Text("\n");
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::Button("SpawnPrefab", ImVec2(150, 30));
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	ImGui::Text("\n");

	int i = ASSET_TYPE::PREFAB;

	static vector<string> vecNames;
	vecNames.clear();
	CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)i, vecNames);

	std::vector<const char*> vecNamePtrs;
	for (auto& name : vecNames)
		vecNamePtrs.push_back(name.c_str());

	ImGui::Combo("##PreFabList", &PreFabObjectIndex, vecNamePtrs.data(), vecNames.size());


	ImGui::Text("\n");
	ImGui::Text("PrefabObjectName");
	ImGui::SameLine(130);
	ImGui::PushItemWidth(150);
	//ImGui::InputText("##PrefabObjectName", PrefabObjectName.data(), PrefabObjectName.size() + 50);
	ImGui::InputText("##PrefabObjectName", PrefabObjectName, IM_ARRAYSIZE(PrefabObjectName));
	ImGui::PopItemWidth();

	ImGui::Text("LayerIndex");
	ImGui::SameLine(100);
	ImGui::InputInt("##LayerPrefabInt", &LayerPreInt);

	if (ImGui::Button("PreFabSpawn", ImVec2(100, 30))) {
		Ptr<CPrefab> Asset = CAssetMgr::GetInst()->FindAsset<CPrefab>(wstring(vecNames[PreFabObjectIndex].begin(), vecNames[PreFabObjectIndex].end()));

		CGameObject* pObject = Asset->Instantiate();
		//wstring wOriginalName = wstring(PrefabObjectName.begin(), PrefabObjectName.end());
		wstring wOriginalName = ToWString(PrefabObjectName);
		pObject->SetName(wOriginalName);
		pObject->Transform()->SetRelativePos(Vec3(0, 0, 0));
		SpawnGameObject(pObject,LayerPreInt);
	}

}

void ObjectEditor::ShowCollisionTable()
{
	const int layerCount = static_cast<int>(COLLISION_LAYER::END);

	if (ImGui::BeginTable("CollisionLayerTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		ImGui::TableSetupColumn("Index");
		ImGui::TableSetupColumn("Layer Name");
		ImGui::TableHeadersRow();

		for (int i = 0; i < static_cast<int>(COLLISION_LAYER::END); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%d", i);  // 첫 번째 열 (Index)

			ImGui::TableNextColumn();
			ImGui::Text(COLLISION_LAYER_STR[(UINT)i]);  // 두 번째 열 (Layer Name)
		}

		ImGui::EndTable();
	}
}
