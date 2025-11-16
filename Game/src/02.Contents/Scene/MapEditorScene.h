#pragma once
#include "01.Base/Actor/Scene.h"
#include "02.Contents/Actor/Tile/TileManager.h"

class CMapEditorScene : public CScene
{
	GENERATE_OBJECT(CMapEditorScene)
public:
	CMapEditorScene() = default;
	~CMapEditorScene() = default;

public:
	void BeginPlay() override;
	void Update(float InDeltaTime) override;
	void CaptureSnapShot() override
	{
		CScene::CaptureSnapShot();

		ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
		ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
		ImGui::InputScalar("TileRow", ImGuiDataType_U64, &TileRow);
		ImGui::InputScalar("TileCol", ImGuiDataType_U64, &TileCol);
		if (ImGui::Button("LayTiles"))
			bLayTiles = true;
		if (ImGui::Button("OpenWindowDialog"))
			bOpenWindowDialog = true;
		{
			ImGui::RadioButton("CenterMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Center));
			ImGui::SameLine();
			ImGui::RadioButton("DownMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Down));
			ImGui::SameLine();
			ImGui::RadioButton("UpMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Up));
			ImGui::SameLine();
			ImGui::RadioButton("LeftMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Left));
			ImGui::SameLine();
			ImGui::RadioButton("RightMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Right));
			ImGui::SameLine();
		}
	}
private:
	// 원래는 모드를 두는게 좋을것같은데 그냥 일단 씬에 때려박자 나중에 ㄱㄱ
	CTileManager* TileManager = nullptr;
	size_t TileWidth = 90;
	size_t TileHeight = 60;
	size_t TileRow = 30;
	size_t TileCol = 30;
	bool bLayTiles = false;
	ETilePutMode TilePutMode = ETilePutMode::Center;

	std::wstring CurrentObjectImagePath = L"";
	bool bOpenWindowDialog = false;

};

