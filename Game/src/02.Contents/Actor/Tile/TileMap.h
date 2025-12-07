#pragma once
#include "01.Base/Actor/Actor.h"
#include "04.Renderer/ImGuiManager.h"
#include "Tile.h"

struct TTileGridData
{
	CTile* Tile;
	CStaticActor* PutOnActor;
};

using TileKey = size_t;

class CTileMap : public CActor
{
	GENERATE_OBJECT(CTileMap)
public:
	CTileMap();
	~CTileMap() = default;

public:
	TileKey GetProximateTile(const Vector2& InWorldPosition)
	{
		for (size_t i = 0; i < TileGridDatas.size(); ++i)
		{
			CTile* Tile = TileGridDatas[i].Tile;
			Vector2 TileCenter = Vector2(Tile->GetTransform()->GetFinalPosition().x, Tile->GetTransform()->GetFinalPosition().y);
			Vector2 TileScale = Vector2(Tile->GetTransform()->GetScale().x, Tile->GetTransform()->GetScale().y);

			float HalfWidth = TileScale.x * 0.5f;
			float HalfHeight = TileScale.y * 0.5f;

			float Left = TileCenter.x - HalfWidth;
			float Right = TileCenter.x + HalfWidth;
			float Top = TileCenter.y - HalfHeight;
			float Bottom = TileCenter.y + HalfHeight;

			if (InWorldPosition.x >= Left && InWorldPosition.x <= Right &&
				InWorldPosition.y >= Top && InWorldPosition.y <= Bottom)
			{
				return i;
			}
		}

		return TileNone;
	}
	CTile* GetTile(TileKey InTileKey) { return TileGridDatas[InTileKey].Tile; }
	CStaticActor* GetPutOnActor(TileKey InTileKey) { return TileGridDatas[InTileKey].PutOnActor; }

	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);

	bool IsValidateKey(TileKey InTileKey) { return InTileKey != TileNone; }
	bool IsActorOnTile(TileKey InTileKey) { return TileGridDatas[InTileKey].PutOnActor; }
	void PutActorOnTile(CStaticActor& InActor, TileKey InTileKey);
	void CutActorOnTile(TileKey InTileKey);

	void RenderTiles(bool bInRender)
	{
		for (auto TileGridData : TileGridDatas)
			TileGridData.Tile->GetSpriteRenderComponent()->SetRender(bInRender);
	}
	size_t GetTileMapRow() const { return TileMapRow; }
	size_t GetTileMapCol() const { return TileMapCol; }
	size_t GetTileWidth() const { return TileWidth; }
	size_t GetTileHeight() const { return TileHeight; }

private:
	std::vector<TTileGridData> TileGridDatas;

	size_t TileWidth = 0;
	size_t TileHeight = 0;
	size_t TileMapRow = 0;
	size_t TileMapCol = 0;

	inline static const size_t TileNone = -1;
};

