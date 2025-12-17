#include "pch.h"
#include "TileEditState.h"

#include "GameCore.h"

CTileEditState::CTileEditState()
{
	
}

void CTileEditState::InitalizeEditState(TTileMapEditContext& InTileMapEditContext)
{
	CEditState::InitalizeEditState(InTileMapEditContext);

	GetTileMapEditContext().TileFocus->Activate(false);

	MoveUIOwner = GetWorld()->SpawnActor<CUI>(this);
	CUI* LeftMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* RightMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* TopMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* BottomMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* CenterMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* LeftTopMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* LeftBottomMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* RightTopMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);
	CUI* RightBottomMoveUI = GetWorld()->SpawnActor<CUI>(MoveUIOwner);

	LeftMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, 0.0f, 0.0f));
	LeftMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::Left, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(LeftMoveUI);

	RightMoveUI->GetTransform()->SetPosition(Vector3(20.0f, 0.0f, 0.0f));
	RightMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::Right, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(RightMoveUI);

	TopMoveUI->GetTransform()->SetPosition(Vector3(0.0f, 20.0f, 0.0f));
	TopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::Top, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(TopMoveUI);

	BottomMoveUI->GetTransform()->SetPosition(Vector3(0.0f, -20.0f, 0.0f));
	BottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::Bottom, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(BottomMoveUI);

	CenterMoveUI->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	CenterMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::Center, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(CenterMoveUI);

	LeftTopMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, 20.0f, 0.0f));
	LeftTopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::LeftTop, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(LeftTopMoveUI);

	LeftBottomMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, -20.0f, 0.0f));
	LeftBottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::LeftBottom, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(LeftBottomMoveUI);

	RightTopMoveUI->GetTransform()->SetPosition(Vector3(20.0f, 20.0f, 0.0f));
	RightTopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::RightTop, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(RightTopMoveUI);

	RightBottomMoveUI->GetTransform()->SetPosition(Vector3(20.0f, -20.0f, 0.0f));
	RightBottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				GetTileMapEditContext().TileHandler->MoveActorByTile(ETilePositionType::RightBottom, GetTileMapEditContext().TileMapper);
		});
	InitalizeMoveUI(RightBottomMoveUI);

	MoveUIOwner->GetTransform()->SetScale(Vector3(20.0f * 3, 20.0f * 3, 1.0f));
	MoveUIOwner->Activate(false);
}
