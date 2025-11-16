#pragma once
#include "01.Base/Actor/Actor.h"
#include "Tile.h"

class CTileManager : public CActor
{
	GENERATE_OBJECT(CTileManager)
public:
	CTileManager() = default;
	~CTileManager() = default;

public:
	void FindFocusTile()
	{
		FocusTile = nullptr;
		for (CTile* Tile : Tiles)
		{
			if (Tile->GetInteractionComponent()->IsMouseFocus())
			{
				FocusTile = Tile;
				break;
			}
		}
	}
	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);
	CTile* GetFocusTile() const { return FocusTile; }

private:
	std::vector<CTile*> Tiles;
	CTile* FocusTile = nullptr;
};

