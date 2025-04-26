#include "pch.h"
#include "TileMapEditor.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CTileRender.h>
#include <Engine/CTransform.h>
#include <Engine/CSprite.h>
#include <Engine/CTexture.h>
#include <Engine/CCollider2D.h>

#include <commdlg.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLayer.h>

static vector<char> TileObjectName(100, '\0');

TileMapEditor::TileMapEditor()
{
	SeeImage = false;
	pTex = nullptr;

}

TileMapEditor::~TileMapEditor()
{
}


void TileMapEditor::Init()
{
	pObject = nullptr;

	wOriginalName = L"gray_image.png";
	wTexturePath = L"Texture\\gray_image.png";
	pTex = CAssetMgr::GetInst()->Load<CTexture>(wOriginalName, wTexturePath);

}

void TileMapEditor::Update_UI()
{
	int a = 0;
}

void TileMapEditor::Render_UI()
{

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::Button("Tile_Setting", ImVec2(150, 30));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::PushItemWidth(200);
	ImGui::Text("TileObjectName");
	ImGui::SameLine(150);
	ImGui::InputText("##TileObjectName", TileObjectName.data(), TileObjectName.size() + 1);
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(200);
	ImGui::DragInt("Map Width", &TileWidth, 1, 1, 100);
	ImGui::DragInt("Map Height", &TileHeight, 1, 1, 100);
	ImGui::PopItemWidth();

	ImGui::PushItemWidth(200);
	ImGui::DragFloat("TileWidthSize", &TileWidthSize, 1, 1, 500);
	ImGui::DragFloat("TileHeightSize", &TileHeightSize, 1, 1, 500);
	ImGui::PopItemWidth();

	static vector<string> vecSpriteName;
	vecSpriteName.clear();
	CAssetMgr::GetInst()->GetAssetName((ASSET_TYPE)SPRITE, vecSpriteName);
	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(wstring(vecSpriteName[SpriteListIndex].begin(), vecSpriteName[SpriteListIndex].end()));
	vector<const char*> itemsSprite;

	for (const auto& name : vecSpriteName) {
		itemsSprite.push_back(name.c_str());
	}

	ImGui::Text("SpriteList");
	ImGui::Combo("##SpriteList", &SpriteListIndex, itemsSprite.data(), itemsSprite.size());
	ImGui::Text("\n");

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	CLayer* pLayer = pLevel->GetLayer(7); //7 Tile
	const vector<CGameObject*>& vecObj = pLayer->GetParentObjects();

	vector<const char*> TileSprite;

	for (int j = 0; j < vecObj.size(); j++) {
		//objNameStr.c_str()는 std::string의 내부 버퍼를 반환하는데, std::vector에 추가되는 것이 아니라, 같은 메모리 주소를 가리키는 포인터를 추가하는 것이야.
		TileSprite.push_back(ToString(vecObj[j]->GetName()).c_str());
	}

	ImGui::Text("TileObjectList");
	ImGui::Combo("##TileObjectList", &TileObjectIndex, TileSprite.data(), TileSprite.size());
	ImGui::Text("\n");

	if (TileObjectIndex != -1) {
		pObject = vecObj[TileObjectIndex];
	}

	ImGui::Text("AtlasName : ");
	ImGui::SameLine(100);
	ImGui::Text(ToString(pSprite->GetAtlasTex()->GetKey()).c_str());

	float LeftTop_x = pSprite->GetLeftTop().x;
	float LeftTop_y = pSprite->GetLeftTop().y;
	float Slice_x = pSprite->GetSlice().x;
	float Slice_y = pSprite->GetSlice().y;

	pTex = pSprite->GetAtlasTex();
	ImVec2 uv_min = ImVec2(LeftTop_x, LeftTop_y);                 // Top-left
	ImVec2 uv_max = ImVec2(LeftTop_x + Slice_x, LeftTop_y + Slice_y);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
		, ImVec2(100.f, 100.f)			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러




	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // 아이템 간 간격을 고정 (수평 간격을 10px로 설정)
	ImGui::Columns(TileWidth, nullptr, false);
	for (int y = 0; y < TileWidth; y++) {
		for (int x = 0; x < TileHeight; x++) {

			//TileObject가 소환돼있으면
			if (pObject != nullptr) {
				//텍스쳐를 받는거고
				Ptr<CSprite> CurSprite = pObject->TileRender()->GetSprite(y,x);

				if (CurSprite == nullptr) {
					CurSprite = CAssetMgr::GetInst()->CAssetMgr::GetInst()->Load<CSprite>(L"Sprite\\emptymapsprite_0.sprite", L"Sprite\\emptymapsprite_0.sprite");
				}

				Ptr<CTexture> TileTex = CurSprite->GetAtlasTex();

				float LeftTop_x_a = CurSprite->GetLeftTop().x;
				float LeftTop_y_a = CurSprite->GetLeftTop().y;
				float Slice_x_a = CurSprite->GetSlice().x;
				float Slice_y_a = CurSprite->GetSlice().y;

				ImVec2 uv_min_cur = ImVec2(LeftTop_x_a, LeftTop_y_a);                 // Top-left
				ImVec2 uv_max_cur = ImVec2(LeftTop_x_a + Slice_x_a, LeftTop_y_a + Slice_y_a);                 // Lower-right
				ImVec4 tint_col_cur = ImVec4(1.f, 1.f, 1.f, 1.0f);
				ImVec4 border_col_cur = ImVec4(0.0f, 0.f, 1.f, 1.0f);

				ImGui::Image((ImTextureID)TileTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
					, ImVec2(50.f, 50.f)			   // 텍스쳐 위젯 크기
					, uv_min_cur, uv_max_cur				   // 출력할 텍스쳐 UV 범위
					, tint_col_cur, border_col_cur);		   // 틴트, 보더 컬러

				//스프라이트를 넣어줘야함 Texture이름에 그대로 대응하는 Sprite를 만들어줘야함
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					//string Originalstr = string(TileImageName.data());
					//Originalstr += sprite;
					//Originalstr = "Sprite\\" + Originalstr;
					//wstring ab = wstring(Originalstr.begin(), Originalstr.end());
					//pObject->TileRender()->SetSprite(y, x, CAssetMgr::GetInst()->FindAsset<CSprite>(ab));
					pObject->TileRender()->SetSprite(y, x, pSprite);
				}
				else if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
					pObject->TileRender()->SetTileRotation(y,x);
				}
			}
		}
		ImGui::NextColumn();
	}
	ImGui::Columns(1); // 열 설정 리셋
	ImGui::PopStyleVar(); // 스타일을 원래대로 복원

	if (ImGui::Button("SpawnTile", ImVec2(70, 30))) {

		pObject = new CGameObject;
		pObject->SetName(wstring(TileObjectName.begin(), TileObjectName.end()));
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CTileRender);
		//pObject->AddComponent(new CCollider2D);

		pObject->Transform()->SetRelativePos(0.f, 0.f, 10.f);
		pObject->Transform()->SetRelativeScale(300.f, 300.f, 1.f);

		//pObject->Collider2D()->SetIndependentScale(true);
		//pObject->Collider2D()->SetScale(Vec2(300.f, 300.f));
		//pObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));

		pObject->TileRender()->SetColRow(TileWidth, TileHeight);
		pObject->TileRender()->SetTileSize(Vec2(TileWidthSize, TileHeightSize));

		//for (int i = 0; i < TileWidth; ++i)
		//{
		//	for (int j = 0; j < TileHeight; ++j)
		//	{
		//		if (i % 2 == 0) {
		//			Ptr<CSprite> CurSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floorbotTilesmap1_7_0.sprite");
		//			pObject->TileRender()->SetSprite(i, j, CurSprite);
		//		}
		//		else {
		//			Ptr<CSprite> CurSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floorbotTilesmap1_6_0.sprite");
		//			pObject->TileRender()->SetSprite(i, j, CurSprite);
		//		}
		//	}
		//}

		//pObject->TileRender()->SetSprite(0, 0, CAssetMgr::GetInst()->FindAsset<CSprite>
		//	(L"Sprite\\floorbotTilesmap1_8_0.sprite"));

		pObject->TileRender()->SetSprite(0, 0, CAssetMgr::GetInst()->FindAsset<CSprite>(L"Sprite\\floortilesprite_0.sprite"));

		SpawnGameObject(pObject, 0);

		pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wTexturePath);

	}
	ImGui::SameLine(100);
	if (ImGui::Button("AnotherTile", ImVec2(70, 30))) {
		pObject = nullptr;
	}

}
