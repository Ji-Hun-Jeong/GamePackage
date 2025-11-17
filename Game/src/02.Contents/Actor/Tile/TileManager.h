#pragma once
#include "01.Base/Actor/Actor.h"
#include "04.Renderer/ImGuiManager.h"
#include "Tile.h"

class CTileManager : public CActor
{
	GENERATE_OBJECT(CTileManager)
public:
	CTileManager() = default;
	~CTileManager() = default;

public:
	CTile* GetProximateTile(const Vector2& InWorldPosition)
	{
		CTile* ProximateTile = nullptr;

		for (size_t i = 0; i < Tiles.size(); ++i)
		{
			CTile* Tile = Tiles[i];
			Vector2 TileCenter = Vector2(Tile->GetTransform()->GetFinalPosition().x, Tile->GetTransform()->GetFinalPosition().y);
			Vector2 TileScale = Vector2(Tile->GetTransform()->GetScale().x, Tile->GetTransform()->GetScale().y);

			// 타일의 경계 계산 (중심 기준으로 좌우/상하 반씩 나눔)
			float HalfWidth = TileScale.x * 0.5f;
			float HalfHeight = TileScale.y * 0.5f;

			float Left = TileCenter.x - HalfWidth;
			float Right = TileCenter.x + HalfWidth;
			float Top = TileCenter.y - HalfHeight;
			float Bottom = TileCenter.y + HalfHeight;

			// 위치가 타일 안에 있는지 확인
			if (InWorldPosition.x >= Left && InWorldPosition.x <= Right &&
				InWorldPosition.y >= Top && InWorldPosition.y <= Bottom)
			{
				ProximateTile = Tile;
				break;
			}
		}

		return ProximateTile;
	}
	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);
	void RenderTiles(bool bInRender)
	{
		for (auto Tile : Tiles)
			Tile->GetSpriteRenderComponent()->SetRender(bInRender);
	}

private:
	std::vector<CTile*> Tiles;

	size_t TileWidth = 0;
	size_t TileHeight = 0;
	size_t TileMapRow = 0;
	size_t TileMapCol = 0;
	ETilePutMode TilePutMode = ETilePutMode::Center;

};

