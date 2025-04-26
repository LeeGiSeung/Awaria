#include "pch.h"
#include "SpriteEditor.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CSprite.h>
#include <Engine/CTexture.h>

#include <commdlg.h>

#include "Content.h"
#include <codecvt>
#include <locale>

int tab = 100;
//static vector<char> OriginalName(256, '\0');
//static vector<char> SpritelName(256, '\0');

static char OriginalName[64] = "";
static char SpritelName[64] = "";

string texturePath = "Texture\\";
string png = ".png";
static bool isSelecting = false;

float normalize(float value, float min, float max) {
	if (max == min) {

		return 0.0f; // 오류 처리
	}
	return (value - min) / (max - min);
}

SpriteEditor::SpriteEditor()
{
	curSpriteIndex = 0;
	LeftTop_x = 0.f;
	LeftTop_y = 0.f;
	Slice_x = 0.f;
	Slice_y = 0.f;
	SeeOriginalImage = false;
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Init()
{
	wOriginalName = L"gray_image.png";
	wTexturePath = L"Texture\\gray_image.png";
}

void SpriteEditor::Update_UI()
{
}

void SpriteEditor::Render_UI()
{

	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::Button("Sprite_Setting", ImVec2(150, 30));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	int tab = 100;
	ImGui::PushItemWidth(200);
	ImGui::Text("OriginalImage");
	//ImGui::InputText("##OriginalImage", OriginalName.data(), OriginalName.size() + 50);
	ImGui::InputText("##OriginalImage", OriginalName, IM_ARRAYSIZE(OriginalName));
	ImGui::PopItemWidth();

	ImGui::SameLine(250);
	if (ImGui::Button("##FindTexture", ImVec2(20, 20)))
	{
		wstring strContentPath = CPathMgr::GetContentPath();
		strContentPath += L"Texture";

		// 파일 경로 문자열
		wchar_t szFilePath[255] = {};

		OPENFILENAME Desc = {};

		Desc.lStructSize = sizeof(OPENFILENAME);
		Desc.hwndOwner = nullptr;
		Desc.lpstrFile = szFilePath;
		Desc.nMaxFile = 255;
		Desc.lpstrFilter = L".png";
		Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		Desc.lpstrInitialDir = strContentPath.c_str();

		if (GetOpenFileName(&Desc))
		{
			wstring filePath = szFilePath;

			// 마지막 '\' (디렉터리 구분자) 찾기
			size_t lastSlash = filePath.find_last_of(L"\\/");

			// 파일 이름만 추출
			wstring fileName = (lastSlash != wstring::npos) ? filePath.substr(lastSlash + 1) : filePath;

			// 마지막 '.' 찾기 (확장자 제거)
			size_t lastDot = fileName.find_last_of(L'.');
			if (lastDot != wstring::npos)
			{
				fileName = fileName.substr(0, lastDot); // 확장자 제외
			}

			// **wstring을 string으로 변환**
			string narrowStr(fileName.begin(), fileName.end());

			// **vector<char>에 복사**
			//fill(OriginalName.begin(), OriginalName.end(), '\0'); // 기존 데이터 초기화
			fill(OriginalName, OriginalName, '\0'); // 기존 데이터 초기화
			//copy(narrowStr.begin(), narrowStr.end(), OriginalName.begin());
			copy(narrowStr.begin(), narrowStr.end(), OriginalName);
		}

		wstring strBinPath = CPathMgr::GetBinPath();
		SetCurrentDirectory(strBinPath.c_str());
	}

	string texturePath = "Texture\\";
	string png = ".png";

	Ptr<CTexture> pTex;

	ImVec2 bt(100, 50);

	if (ImGui::Button("LoadingImage", bt)) {
		SeeOriginalImage = true;
	};

	//Sprite 제작 성공 AssetUI 업데이트 진행하면됨
	ImGui::SameLine(150);
	if (ImGui::Button("MakeSprite", bt)) {

		Ptr<CSprite> pSprite = new CSprite;
		pSprite->SetAtlasTex(CAssetMgr::GetInst()->FindAsset<CTexture>(wOriginalName));
		pSprite->SetLeftTop(Vec2(LeftTop_x, LeftTop_y));
		pSprite->SetSlice(Vec2(Slice_x, Slice_y));
		float MaxBackground;

		if (Slice_x - LeftTop_x > Slice_y - LeftTop_y) {
			MaxBackground = Slice_x - LeftTop_x;
		}
		else {
			MaxBackground = Slice_y - LeftTop_y;
		}

		pSprite->SetBackground(Vec2(MaxBackground, MaxBackground));


		string SpriteNamestr = string(SpritelName, strnlen(SpritelName, IM_ARRAYSIZE(SpritelName)));
		SpriteNamestr = "Sprite\\" + SpriteNamestr;
		wchar_t SpriteName[255] = {};
		swprintf_s(SpriteName, 255, L"%hs_%d", SpriteNamestr.c_str(), curSpriteIndex);

		CAssetMgr::GetInst()->AddAsset(SpriteName, pSprite.Get());
		curSpriteIndex++;

		pSprite->Save(SpriteName);

	};
	//string SpriteNamestr = string(SpritelName.data(), strnlen(SpritelName.data(), SpritelName.size()));
	ImGui::SameLine(300);
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.3f, 0.3f, 1.f));
	if (ImGui::Button("auto", ImVec2(50, 50))) {
		if (autoSet) {
			autoSet = false;
		}
		else {
			autoSet = true;
		}
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	//ImGui::SameLine(150);
	//if (ImGui::Button("CancleLoad", bt)) {
	//	SeeOriginalImage = false;
	//};

	if (SeeOriginalImage && !string(OriginalName).empty()) {

		//string Originalstr = string(OriginalName.data());
		string Originalstr = string(OriginalName);

		texturePath += Originalstr;
		texturePath += png;

		Originalstr += png;

		//wOriginalName = wstring(OriginalName.begin(), OriginalName.end());
		wOriginalName = ToWString(OriginalName);
		wTexturePath = wstring(texturePath.begin(), texturePath.end());

		SeeOriginalImage = false;

	}
	pTex = CAssetMgr::GetInst()->Load<CTexture>(wOriginalName, wTexturePath);

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
	ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	ImVec4 border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
		, ImVec2(pTex->GetWidth(), pTex->GetHeight())			   // 텍스쳐 위젯 크기
		, uv_min, uv_max				   // 출력할 텍스쳐 UV 범위
		, tint_col, border_col);		   // 틴트, 보더 컬러
	ImGui::SameLine(pTex->GetWidth() + 20);

	ImVec2 cur_uv_min;
	ImVec2 cur_uv_max;

	//자동 설정 했으면
	if (autoSet) {
		Slice_x = pTex->GetWidth();
		Slice_y = pTex->GetHeight();
	}

	ImVec2 WidgetLeftTop = ImGui::GetItemRectMin(); //위젯 좌측 상단
	if (ImGui::IsItemClicked(0)) { //좌클릭했을때 감지

		ImVec2 CurMousePos = ImGui::GetMousePos(); // 화면 상 마우스 위치
		ImVec2 WigetSize = ImGui::GetItemRectSize();

		float scaleX = pTex->GetWidth() / WigetSize.x;
		float scaleY = pTex->GetHeight() / WigetSize.y;

		//현재 위젯의 정규화 0~1 UV 값
		//float WidgetLeftTop_x = normalize(0, 0, WigetSize.x);
		//float WidgetLeftTop_y = normalize(0, 0, WigetSize.y);
		//float WidgetRightDown_x = normalize(WigetSize.x, 0, WigetSize.x);
		//float WidgetRightDown_y = normalize(WigetSize.y, 0, WigetSize.y);

		MouseSplitCount++;

		CurMousePos.x -= WidgetLeftTop.x;
		CurMousePos.y -= WidgetLeftTop.y;

		CurMousePos.x *= scaleX;
		CurMousePos.y *= scaleY;

		if (MouseSplitCount == 1) {
			RedRectLeftTop = ImGui::GetMousePos();
			isSelecting = true;

			LeftTop_x = CurMousePos.x;
			LeftTop_y = CurMousePos.y;
		}
		else if (MouseSplitCount == 2) {
			Slice_x = CurMousePos.x;
			Slice_y = CurMousePos.y;

			Slice_x -= LeftTop_x; //LeftTop은 확실한 왼쪽 위 좌표
			Slice_y -= LeftTop_y;

			MouseSplitCount = 0;
			isSelecting = false;
		}
	};
	if (isSelecting) {
		ImVec2 CurRedMousePos = ImGui::GetMousePos();

		ImDrawList* drawList = ImGui::GetForegroundDrawList();

		drawList->AddRect(
			RedRectLeftTop,
			CurRedMousePos,
			IM_COL32(255, 0, 0, 255),
			0.0f, 0, 2.0f
		);
	}

	cur_uv_min = ImVec2(normalize(LeftTop_x, 0, pTex->GetWidth()), normalize(LeftTop_y, 0, pTex->GetHeight()));
	cur_uv_max = ImVec2(
		cur_uv_min.x + normalize(Slice_x, 0, pTex->GetWidth()),
		cur_uv_min.y + normalize(Slice_y, 0, pTex->GetHeight())
	);

	tint_col = ImVec4(1.f, 1.f, 1.f, 1.0f);
	border_col = ImVec4(0.0f, 0.f, 1.f, 1.0f);

	ImGui::Image((ImTextureID)pTex->GetSRV().Get() // 렌더링할 텍스쳐 ShaderResourceView
		, ImVec2(Slice_x, Slice_y)				// 텍스쳐 위젯 크기
		, cur_uv_min, cur_uv_max			// 출력할 텍스쳐 UV 범위
		, tint_col, border_col);			// 틴트, 보더 컬러

	/*ImGui::InputInt("##MouseSplitCount", &MouseSplitCount);*/

	// 텍스쳐 해상도
	char buff[255] = {};

	ImGui::Text("SpriteName");
	ImGui::SameLine(100);
	ImGui::InputText("##SpriteName", SpritelName, 50);

	ImGui::Text("SpriteIndex");
	ImGui::SameLine(100);
	ImGui::InputInt("##CameraPriority", &curSpriteIndex);

	ImGui::Text("LeftTop_x");
	ImGui::SameLine(100);
	ImGui::InputFloat("##SpriteLeftTop_x", &LeftTop_x);

	ImGui::Text("LeftTop_y");
	ImGui::SameLine(100);
	ImGui::InputFloat("##SpriteLeftTop_y", &LeftTop_y);

	ImGui::Text("Slice_x");
	ImGui::SameLine(100);
	ImGui::InputFloat("##Slice_x", &Slice_x);

	ImGui::Text("Slice_y");
	ImGui::SameLine(100);
	ImGui::InputFloat("##Slice_y", &Slice_y);

	//ImGui::GetMousePos();

}
