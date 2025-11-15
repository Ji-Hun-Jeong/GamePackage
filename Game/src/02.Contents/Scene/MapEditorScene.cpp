#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"
#include "02.Contents/Actor/Tile/TileManager.h"

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(0.0f);
	CTileManager* TileManager = GetWorld()->SpawnActor<CTileManager>();
	TileManager->LayTiles(60, 60, 5, 5);
}
