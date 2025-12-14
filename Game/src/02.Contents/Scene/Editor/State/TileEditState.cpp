#include "pch.h"
#include "TileEditState.h"

#include "GameCore.h"

CTileEditState::CTileEditState()
{
	TileFocus = GetWorld()->SpawnActor<CTileFocus>(this);
	TileHandler = GetWorld()->SpawnActor<CTileHandler>(this);

	CUI* LeftMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* RightMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* TopMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* BottomMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* CenterMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* LeftTopMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* LeftBottomMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* RightTopMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);
	CUI* RightBottomMoveUI = GetWorld()->SpawnActor<CUI>(nullptr);

	LeftMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, 0.0f, 0.0f));
	LeftMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::Left, TileMapper);
		});
	InitalizeMoveUI(LeftMoveUI);

	RightMoveUI->GetTransform()->SetPosition(Vector3(20.0f, 0.0f, 0.0f));
	RightMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::Right, TileMapper);
		});
	InitalizeMoveUI(RightMoveUI);

	TopMoveUI->GetTransform()->SetPosition(Vector3(0.0f, 20.0f, 0.0f));
	TopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::Top, TileMapper);
		});
	InitalizeMoveUI(TopMoveUI);

	BottomMoveUI->GetTransform()->SetPosition(Vector3(0.0f, -20.0f, 0.0f));
	BottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::Bottom, TileMapper);
		});
	InitalizeMoveUI(BottomMoveUI);

	CenterMoveUI->GetTransform()->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	CenterMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::Center, TileMapper);
		});
	InitalizeMoveUI(CenterMoveUI);

	LeftTopMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, 20.0f, 0.0f));
	LeftTopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::LeftTop, TileMapper);
		});
	InitalizeMoveUI(LeftTopMoveUI);

	LeftBottomMoveUI->GetTransform()->SetPosition(Vector3(-20.0f, -20.0f, 0.0f));
	LeftBottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::LeftBottom, TileMapper);
		});
	InitalizeMoveUI(LeftBottomMoveUI);

	RightTopMoveUI->GetTransform()->SetPosition(Vector3(20.0f, 20.0f, 0.0f));
	RightTopMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::RightTop, TileMapper);
		});
	InitalizeMoveUI(RightTopMoveUI);

	RightBottomMoveUI->GetTransform()->SetPosition(Vector3(20.0f, -20.0f, 0.0f));
	RightBottomMoveUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
				TileHandler->MoveActorByTile(ETilePositionType::RightBottom, TileMapper);
		});
	InitalizeMoveUI(RightBottomMoveUI);
}
