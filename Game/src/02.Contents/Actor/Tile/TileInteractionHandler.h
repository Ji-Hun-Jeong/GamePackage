#pragma once
#include "TileMap.h"

class CTileInteractionHandler
{
public:
	CTileInteractionHandler(CTileMap& InTileMap)
		: TileMap(InTileMap)
	{}
	~CTileInteractionHandler() = default;

public:
	/*bool PutActorOnProximateTile(class CActorGenerator& InActorGenerator, class CTileMap& InTileManager, const Vector2& InWorldPosition);
	bool CutActorOnProximateTile(class CActorGenerator& InActorGenerator, class CTileMap& InTileManager, const Vector2& InWorldPosition);*/
	void MoveHandledTiles(ETilePositionType InTilePositionType);
	bool IsHandledTileEmpty() const { return HandledTiles.empty(); }
	Vector2 GetCenterPositionByHandledTiles()
	{
		Vector2 CenterPosition(0.0f);
		for (TileKey TileKey : HandledTiles)
		{
			CTile* Tile = TileMap.GetTile(TileKey);
			CenterPosition += Tile->GetTransform()->GetFinalPosition2D();
		}

		CenterPosition /= float(HandledTiles.size());
		return CenterPosition;
	}
	bool AdjustTileSnapUIPosition(class CTileSnapUI& InTileSnapUI);
	void SetGroundByHandledTiles(class CGroundManager& InGroundManager);
	void ClearHandledTiles();
	bool AddHandledTile(TileKey InTileKey)
	{
		for (TileKey TileKey : HandledTiles)
			if (TileKey == InTileKey)
				return false;

		CTile* Tile = TileMap.GetTile(InTileKey);
		Tile->ChangeEdge(Vector3(0.0f, 1.0f, 0.0f));
		Tile->GetSpriteRenderComponent()->SetLayer(2);
		HandledTiles.push_back(InTileKey);
		return true;
	}
	bool RemoveHandledTile(TileKey InTileKey)
	{
		for (auto Iter = HandledTiles.begin(); Iter != HandledTiles.end(); ++Iter)
		{
			if (*Iter == InTileKey)
			{
				HandledTiles.erase(Iter);
				TileMap.GetTile(InTileKey)->RevertEdge();
				TileMap.GetTile(InTileKey)->GetSpriteRenderComponent()->SetLayer(1);
				return true;
			}
		}
		return false;
	}

private:
	std::vector<TileKey> HandledTiles;
	CTileMap& TileMap;

};

