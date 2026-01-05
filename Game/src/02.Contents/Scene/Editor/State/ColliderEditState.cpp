#include "pch.h"
#include "ColliderEditState.h"

#include "GameCore.h"

void CColliderEditState::BeginPlay()
{
	CEditState::BeginPlay();

	GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);
	DrawCollider = GetWorld()->SpawnActor<CStaticActor>(this);
	DrawCollider->SetLineActor();
	DrawCollider->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);
}

void CColliderEditState::DrawMode()
{
	CUI* MainPanel = GetTileMapEditContext().MainPanel;
	CTileMap* TileMap = GetTileMapEditContext().TileMap;
	CTileMapper& TileMapper = GetTileMapEditContext().TileMapper;
	CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
	CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

	if (bOnMainPanel == false)
		return;
	bOnMainPanel = false;

	const Vector2& MousePosition = GetMouseWorldPosition();
	if (LClicked())
	{
		LeftTopPosition = Vector3(MousePosition.x, MousePosition.y, 1.0f);
		DrawCollider->GetTransform()->SetScale(Vector3(0.0f, 0.0f, 0.0f));
		DrawCollider->GetTransform()->SetPosition(LeftTopPosition);
		DrawCollider->GetSpriteRenderComponent()->SetRender(true);
	}
	else if (LHold())
	{
		Vector3 RightBottomPosition = Vector3(MousePosition.x, MousePosition.y, 1.0f);
		Vector3 Position = (LeftTopPosition + RightBottomPosition) / 2.0f;
		Vector3 Scale = RightBottomPosition - LeftTopPosition;
		Scale.x = Scale.x >= 0.0f ? Scale.x : -Scale.x;
		Scale.y = Scale.y >= 0.0f ? Scale.y : -Scale.y;
		DrawCollider->GetTransform()->SetScale(Scale);
		DrawCollider->GetTransform()->SetPosition(Position);
	}
	else if (LReleased())
	{
		DrawCollider->GetSpriteRenderComponent()->SetRender(false);

		const Vector3& Position = DrawCollider->GetTransform()->GetFinalPosition();
		const Vector3& Scale = DrawCollider->GetTransform()->GetScale();
		GroundManager->AddGround(Position, Scale);
	}
	else if (RHold())
	{
		GroundManager->RemoveProximateGround(MousePosition);
	}
}

void CColliderEditState::PlaceMode()
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
	bOnMainPanel = false;

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
		bool bSuccessRemoveCollider = GroundManager->RemoveProximateGround(MouseWorldPosition);
		if (bSuccessRemoveCollider)
			TileHandler->ClearHandledTiles();
		else
			TileHandler->EraseHandledTile(*FocusTile);
	}
}
