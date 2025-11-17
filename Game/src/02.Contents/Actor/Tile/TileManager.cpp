#include "pch.h"
#include "TileManager.h"
#include "GameCore.h"

#include "04.Renderer/ImGuiManager.h"

void CTileManager::LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol)
{
	for (CTile* Tile : Tiles)
		Tile->Destroy();
	Tiles.clear();

	TileWidth = InWidth;
	TileHeight = InHeight;
	TileMapRow = InRow;
	TileMapCol = InCol;

	float FirstX = (-(InCol - 1.0f) / 2.0f) * InWidth;
	float FirstY = ((InRow - 1.0f) / 2.0f) * InHeight;
	float Z = 1.0f;
	Vector3 FirstPosition = Vector3(FirstX, FirstY, Z);
	Vector3 Scale = Vector3(float(InWidth), float(InHeight), 1.0f);

	Tiles.resize(InRow * InCol, nullptr);
	for (size_t i = 0; i < InRow; ++i)
	{
		for (size_t j = 0; j < InCol; ++j)
		{
			CTile*& Tile = Tiles[i * InCol + j];
			Tile = GetWorld()->SpawnActor<CTile>();
			Vector3 Position = FirstPosition + Vector3(float(InWidth * j), -float(InHeight * i), 0.0f);
			Tile->GetTransform()->SetPosition(Position);
			Tile->GetTransform()->SetScale(Scale);
		}
	}
}
