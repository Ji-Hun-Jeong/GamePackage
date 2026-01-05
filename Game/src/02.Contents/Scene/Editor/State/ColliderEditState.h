#pragma once
#include "IEditState.h"
#include "02.Contents/Actor/Manager/GroundManager.h"

class CColliderTemporaryMarker : public CStaticActor
{
	GENERATE_OBJECT(CColliderTemporaryMarker)
public:
	CColliderTemporaryMarker() = default;
	~CColliderTemporaryMarker() = default;

public:

private:

};
class CColliderEditState : public CEditState
{
	GENERATE_OBJECT(CColliderEditState)
public:
	CColliderEditState() = default;
	~CColliderEditState() = default;

public:
	void BeginPlay() override;
	void EnterEditState() override
	{
		GetTileMapEditContext().MainPanel->SetMouseFocusEvent([this]()->void {bOnMainPanel = true; });
	}
	void OnEditState() override
	{
		if (bDrawMode)
			DrawMode();
		else
			PlaceMode();
	}
	void ExitEditState() override
	{
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CTileMapper& TileMapper = GetTileMapEditContext().TileMapper;
		CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
		CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

		MainPanel->SetMouseFocusEvent(nullptr);

		TileFocus->SetFocusTile(nullptr, 0);
		TileHandler->ClearHandledTiles();
	}
	void ClearEditState() override
	{
		GroundManager->ClearGrounds();
	}
	void ToImGUI() override
	{
		if (ImGui::Button("DrawMode"))
		{
			CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
			CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

			TileFocus->SetFocusTile(nullptr, 0);
			TileHandler->ClearHandledTiles();
			bDrawMode = true;
		}
		if (ImGui::Button("PlaceMode"))
			bDrawMode = false;

		if (ImGui::Button("PlaceGround"))
			bPlaceGround = true;
	}

private:
	void DrawMode();
	void PlaceMode();

private:
	CGroundManager* GroundManager = nullptr;
	bool bPlaceGround = false;

	bool bOnMainPanel = false;

	bool bDrawMode = false;

	CStaticActor* DrawCollider = nullptr;
	Vector3 LeftTopPosition;

};

