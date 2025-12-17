#pragma once
#include "IEditState.h"
#include "02.Contents/Actor/Collision/GroundManager.h"

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
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CTileMapper& TileMapper = GetTileMapEditContext().TileMapper;
		CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
		CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

		if (bPlaceGround)
		{
			if (TileHandler->IsEmpty() == false)
				TileHandler->SetGroundByHandledTiles(*TileMap, *GroundManager);
			bPlaceGround = false;
		}

		if (bOnMainPanel == false)
			return;

		const Vector2& MouseWorldPosition = GetMouseWorldPosition();
		CTile* FocusTile = TileMap->GetTileByPosition(MouseWorldPosition);
		if (FocusTile == nullptr)
			return;

		TileFocus->SetFocusTile(FocusTile, FocusTile->GetSpriteRenderComponent()->GetLayer() + 1);

		if (LHold())
		{
			if (TileHandler->IsExist(*FocusTile) == false)
				TileHandler->HandleTile(*FocusTile, FocusTile->GetSpriteRenderComponent()->GetLayer() + 2);
		}
		else if (RHold())
		{
			bool bSuccessRemoveCollider = GroundManager->RemoveProximateCollider(MouseWorldPosition);
			if (bSuccessRemoveCollider)
				TileHandler->ClearHandledTiles();
			else
				TileHandler->EraseHandledTile(*FocusTile);
		}

		bOnMainPanel = false;
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
		GroundManager->ClearColliders();
	}
	void ToImGUI() override
	{
		if (ImGui::Button("PlaceGround"))
			bPlaceGround = true;
	}

private:
	CGroundManager* GroundManager = nullptr;
	bool bPlaceGround = false;

	bool bOnMainPanel = false;
};

