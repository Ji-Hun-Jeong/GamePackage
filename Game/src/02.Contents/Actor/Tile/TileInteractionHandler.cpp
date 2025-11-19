#include "pch.h"
#include "TileInteractionHandler.h"
#include "GameCore.h"

#include "02.Contents/Actor/Edit/ActorGenerator.h"
#include "TileManager.h"
#include "TileSnapUI.h"

CTile* CTileInteractionHandler::PutActorOnProximateTile(CActorGenerator& InActorGenerator, CTileManager& InTileManager, const Vector2& InWorldPosition)
{
	CTile* ProximateTile = InTileManager.GetProximateTile(InWorldPosition);
	if (ProximateTile == nullptr)
		return nullptr;

	AddTile(*ProximateTile);

	Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->GetFinalPosition2D();
	const CStaticActor* TilePutOnActor = ProximateTile->GetPutOnActor();
	if (TilePutOnActor)
	{
		const std::wstring& TilePutOnActorImagePath = TilePutOnActor->GetSpriteRenderComponent()->GetImagePath();
		const std::wstring& GeneratedActorImagePath = InActorGenerator.GetGeneratedActorImagePath();
		if (TilePutOnActorImagePath == GeneratedActorImagePath)
			return ProximateTile;
	}

	CStaticActor* PutOnActor = ProximateTile->GetPutOnActor();
	if (PutOnActor)
	{
		InActorGenerator.EraseActor(*ProximateTile->GetPutOnActor());
		ProximateTile->SetPutOnActor(nullptr);
	}

	CStaticActor* GeneratedActor = InActorGenerator.GenerateStaticActor(ProximateTileWorldPosition);
	ProximateTile->SetPutOnActor(GeneratedActor);

	return ProximateTile;
}

CTile* CTileInteractionHandler::CutActorOnProximateTile(CActorGenerator& InActorGenerator, CTileManager& InTileManager, const Vector2& InWorldPosition)
{
	CTile* ProximateTile = InTileManager.GetProximateTile(InWorldPosition);
	if (ProximateTile == nullptr)
		return nullptr;

	InActorGenerator.EraseActor(*ProximateTile->GetPutOnActor());
	ProximateTile->SetPutOnActor(nullptr);

	for (auto Iter = HandledTiles.begin(); Iter != HandledTiles.end(); ++Iter)
	{
		if (*Iter == ProximateTile)
		{
			HandledTiles.erase(Iter);
			break;
		}
	}

	return ProximateTile;
}

void CTileInteractionHandler::MoveHandledTiles(ETilePositionType InTilePositionType)
{
	for (CTile* Tile : HandledTiles)
		Tile->MoveActor(InTilePositionType);
}

bool CTileInteractionHandler::AdjustTileSnapUIPosition(CTileSnapUI& InTileSnapUI)
{
	if (HandledTiles.empty())
		return false;

	Vector2 CenterPosition(0.0f);
	for (CTile* Tile : HandledTiles)
		CenterPosition += Tile->GetTransform()->GetFinalPosition2D();
	
	CenterPosition /= float(HandledTiles.size());

	InTileSnapUI.GetTransform()->SetPosition(Vector3(CenterPosition.x, CenterPosition.y, 0.0f));

	return true;
}

void CTileInteractionHandler::ClearHandledTiles()
{
	HandledTiles.clear();
}
