#pragma once
#include "Tile.h"

class CTileInteractionHandler
{
public:
	CTileInteractionHandler() = default;
	~CTileInteractionHandler() = default;

public:
	CTile* PutActorOnProximateTile(class CActorGenerator& InActorGenerator, class CTileManager& InTileManager, const Vector2& InWorldPosition);
	CTile* CutActorOnProximateTile(class CActorGenerator& InActorGenerator, class CTileManager& InTileManager, const Vector2& InWorldPosition);
	void MoveHandledTiles(ETilePositionType InTilePositionType);
	bool AdjustTileSnapUIPosition(class CTileSnapUI& InTileSnapUI);
	void ClearHandledTiles();

private:
	void AddTile(CTile& InTile)
	{
		for (CTile* Tile : HandledTiles)
			if (Tile == &InTile)
				return;
		HandledTiles.push_back(&InTile);
	}

private:
	std::vector<CTile*> HandledTiles;

};

