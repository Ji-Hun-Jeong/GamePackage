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
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		if (LClicked() == false)
			return;

		CTile* FocusTile = nullptr;
		for (CTile* Tile : Tiles)
		{
			if (Tile->GetInteractionComponent()->IsMouseFocus())
			{
				FocusTile = Tile;
				break;
			}
		}
		
		std::cout << FocusTile << std::endl;
	}
	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);

private:
	std::vector<CTile*> Tiles;

};

