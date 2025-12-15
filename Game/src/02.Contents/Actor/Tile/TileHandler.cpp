#include "pch.h"
#include "TileHandler.h"
#include "GameCore.h"

//void CTileInteractionHandler::MoveHandledTiles(ETilePositionType InTilePositionType)
//{
//	for (TileKey TileKey : HandledTiles)
//	{
//		CTile* Tile = TileMap.GetTile(TileKey);
//		CStaticActor* PutOnActor = TileMap.GetPutOnActor(TileKey);
//		Tile->MoveActor(*PutOnActor, InTilePositionType);
//	}
//}
//
//bool CTileInteractionHandler::AdjustTileSnapUIPosition(CTileSnapUI& InTileSnapUI)
//{
//	if (IsHandledTileEmpty())
//		return false;
//	if (HandledTiles.empty())
//		return false;
//
//	Vector2 CenterPosition = GetCenterPositionByHandledTiles();
//
//	InTileSnapUI.GetTransform()->SetPosition(Vector3(CenterPosition.x, CenterPosition.y, 0.0f));
//
//	return true;
//}
//
//void CTileInteractionHandler::SetGroundByHandledTiles(CGroundManager& InGroundManager)
//{
//	std::sort(HandledTiles.begin(), HandledTiles.end());
//	size_t TileMapRow = TileMap.GetTileMapRow();
//	size_t TileMapCol = TileMap.GetTileMapCol();
//
//	bool bSquare = true;
//	TileKey RowFirstValue = 0;
//	size_t PrevColCount = 0;
//	size_t ColCount = 0;
//	for (size_t i = 0; i < HandledTiles.size(); ++i)
//	{
//		TileKey Key = HandledTiles[i];
//		ColCount += 1;
//		// 맨 끝인 상황
//		if (HandledTiles.size() - 1 == i || HandledTiles[i - ColCount + 1] + TileMapCol == HandledTiles[i + 1])
//		{
//			if (PrevColCount == 0)
//				PrevColCount = ColCount;
//			if (PrevColCount != ColCount)
//			{
//				bSquare = false;
//				break;
//			}
//			ColCount = 0;
//		}
//	}
//
//	if (bSquare)
//	{
//		size_t Row = HandledTiles.size() / PrevColCount;
//		size_t Col = PrevColCount;
//		size_t TileWidth = TileMap.GetTileWidth();
//		size_t TileHeight = TileMap.GetTileHeight();
//
//		Vector2 CenterPosition = GetCenterPositionByHandledTiles();
//		size_t ColliderWidth = TileWidth * Col;
//		size_t ColliderHeight = TileHeight * Row;
//
//		InGroundManager.AddGroundCollider(Vector3(CenterPosition.x, CenterPosition.y, 1.0f), Vector3(ColliderWidth, ColliderHeight, 1.0f));
//	}
//	else
//	{
//		for (TileKey TileKey : HandledTiles)
//		{
//			CTile* Tile = TileMap.GetTile(TileKey);
//			const Vector3& TileFinalPosition = Tile->GetTransform()->GetFinalPosition();
//			const Vector3& TileScale = Tile->GetTransform()->GetScale();
//
//			InGroundManager.AddGroundCollider(TileFinalPosition, TileScale);
//		}
//	}
//
//	for (TileKey TileKey : HandledTiles)
//	{
//		CTile* Tile = TileMap.GetTile(TileKey);
//		Tile->RevertEdge();
//	}
//
//	HandledTiles.clear();
//}
//
//void CTileInteractionHandler::ClearHandledTiles()
//{
//	for (TileKey TileKey : HandledTiles)
//	{
//		CTile* Tile = TileMap.GetTile(TileKey);
//		Tile->RevertEdge();
//	}
//	HandledTiles.clear();
//}
//

void CTileHandler::HandleTile(CTile& InHandledTile, uint32_t InLayer)
{
	HandledTiles.push_back(&InHandledTile);

	CUI* TileMarker = GetWorld()->SpawnActor<CUI>(this);
	TileMarker->GetTransform()->SetPosition(InHandledTile.GetTransform()->GetFinalPosition());
	TileMarker->GetTransform()->SetScale(InHandledTile.GetTransform()->GetScale());
	TileMarker->SetLineActor();
	TileMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);
	TileMarker->GetSpriteRenderComponent()->SetLayer(InLayer);

	TileMarkers.push_back(TileMarker);
}

Vector3 CTileHandler::GetCenterPosition() const
{
	Vector3 CenterPosition;
	for (auto Tile : HandledTiles)
		CenterPosition += Tile->GetTransform()->GetFinalPosition();
	return CenterPosition / float(HandledTiles.size());
}
