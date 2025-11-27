#include "pch.h"
#include "TileInteractionHandler.h"
#include "GameCore.h"

#include "02.Contents/Actor/Edit/ActorGenerator.h"
#include "TileMap.h"
#include "TileSnapUI.h"

//bool CTileInteractionHandler::PutActorOnProximateTile(CActorGenerator& InActorGenerator, CTileMap& InTileManager, const Vector2& InWorldPosition)
//{
//	TileKey ProximateTileKey = InTileManager.GetProximateTile(InWorldPosition);
//	if (InTileManager.IsValidateKey(ProximateTileKey))
//		return false;
//
//	CTile* ProximateTile = InTileManager.GetTile(ProximateTileKey);
//	CStaticActor* TilePutOnActor = InTileManager.GetPutOnActor(ProximateTileKey);
//	if (TilePutOnActor)
//	{
//		const std::wstring& TilePutOnActorImagePath = TilePutOnActor->GetSpriteRenderComponent()->GetImagePath();
//		const std::wstring& GeneratedActorImagePath = InActorGenerator.GetGeneratedActorImagePath();
//		if (TilePutOnActorImagePath == GeneratedActorImagePath)
//		{
//			AddTile(*ProximateTile);
//			return ProximateTile;
//		}
//	}
//
//	if (TilePutOnActor)
//	{
//		InActorGenerator.EraseActor(*TilePutOnActor);
//		InTileManager.CutActorOnTile(ProximateTileKey);
//	}
//
//	// Todo: Generate nullptr
//	Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->GetFinalPosition2D();
//	CStaticActor* GeneratedActor = InActorGenerator.GenerateStaticActor(ProximateTileWorldPosition);
//	if (GeneratedActor)
//	{
//		AddTile(*ProximateTile);
//		InTileManager.PutActorOnTile(*GeneratedActor, ProximateTileKey);
//	}
//
//	return ProximateTile;
//}
//
//bool CTileInteractionHandler::CutActorOnProximateTile(CActorGenerator& InActorGenerator, CTileMap& InTileManager, const Vector2& InWorldPosition)
//{
//	TileKey ProximateTileKey = InTileManager.GetProximateTile(InWorldPosition);
//	if (InTileManager.IsValidateKey(ProximateTileKey))
//		return false;
//
//	CTile* ProximateTile = InTileManager.GetTile(ProximateTileKey);
//	CStaticActor* TilePutOnActor = InTileManager.GetPutOnActor(ProximateTileKey);
//
//	InActorGenerator.EraseActor(*TilePutOnActor);
//	InTileManager.CutActorOnTile(ProximateTileKey);
//
//	for (auto Iter = HandledTiles.begin(); Iter != HandledTiles.end(); ++Iter)
//	{
//		if (*Iter == ProximateTile)
//		{
//			HandledTiles.erase(Iter);
//			break;
//		}
//	}
//
//	return ProximateTile;
//}

void CTileInteractionHandler::MoveHandledTiles(ETilePositionType InTilePositionType)
{
	for (TileKey TileKey : HandledTiles)
	{
		CTile* Tile = TileMap.GetTile(TileKey);
		CStaticActor* PutOnActor = TileMap.GetPutOnActor(TileKey);
		Tile->MoveActor(*PutOnActor, InTilePositionType);
	}		
}

bool CTileInteractionHandler::AdjustTileSnapUIPosition(CTileSnapUI& InTileSnapUI)
{
	if (HandledTiles.empty())
		return false;

	Vector2 CenterPosition(0.0f);
	for (TileKey TileKey : HandledTiles)
	{
		CTile* Tile = TileMap.GetTile(TileKey);
		CenterPosition += Tile->GetTransform()->GetFinalPosition2D();
	}		
	
	CenterPosition /= float(HandledTiles.size());

	InTileSnapUI.GetTransform()->SetPosition(Vector3(CenterPosition.x, CenterPosition.y, 0.0f));

	return true;
}

void CTileInteractionHandler::ClearHandledTiles()
{
	for (TileKey TileKey : HandledTiles)
	{
		CTile* Tile = TileMap.GetTile(TileKey);
		Tile->RevertEdge();
	}
	HandledTiles.clear();
}

