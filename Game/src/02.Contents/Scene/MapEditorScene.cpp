#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(0.0f);

	TileManager = GetWorld()->SpawnActor<CTileManager>();
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	TileManager->FindFocusTile();
	CTile* FocusTile = TileManager->GetFocusTile();
	if (FocusTile && LHold() && CurrentObjectImagePath.empty() == false)
		FocusTile->PutOnActor(CurrentObjectImagePath, TilePutMode);
	if (FocusTile && RHold())
		FocusTile->RevertPutOn();

	if (bLayTiles)
	{
		TileManager->LayTiles(TileWidth, TileHeight, TileRow, TileCol);
		bLayTiles = false;
	}

	if (bOpenWindowDialog)
	{
		if (CWindowManager::GetInst().TryOpenFileDialog())
			CurrentObjectImagePath = CWindowManager::GetInst().GetOpenFilePath();
		bOpenWindowDialog = false;
	}
}
