#pragma once
#include "01.Base/Actor/Actor.h"
#include "02.Contents/Actor/UI/UI.h"
#include "03.Utils/CTransformUtils.h"
#include "04.Renderer/ImGuiManager.h"

class CTile : public CStaticActor
{
	GENERATE_OBJECT(CTile)
public:
	CTile()
	{
		static const Graphics::TMeshData& LineSquareMeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
		SpriteRenderComponent->SetMesh(LineSquareMeshData);
		SpriteRenderComponent->SetPSO(EPSOType::Line);
		SpriteRenderComponent->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	}
	~CTile() = default;

public:

};

class CTileMap : public CActor
{
	GENERATE_OBJECT(CTileMap)
public:
	CTileMap() = default;
	~CTileMap() = default;

public:
	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);
	CTile* GetTileByPosition(const Vector2& InPosition);
	void RenderTiles(bool bInRender)
	{
		for (auto Tile : Tiles)
			Tile->GetSpriteRenderComponent()->SetRender(bInRender);
	}
	size_t GetTileIndex(const CTile& InTile) const
	{
		for (size_t i = 0; i < Tiles.size(); ++i)
		{
			if (Tiles[i] == &InTile)
				return i;
		}
		return TileNone;
	}
	size_t GetTileMapRow() const { return TileMapRow; }
	size_t GetTileMapCol() const { return TileMapCol; }
	size_t GetTileWidth() const { return TileWidth; }
	size_t GetTileHeight() const { return TileHeight; }

	void AddTileMapActor(CActor& InActor)
	{
		for (auto Actor : TileMapActors)
		{
			if (Actor == &InActor)
				return;
		}
		TileMapActors.push_back(&InActor);
	}
	void ClearTileMap()
	{
		for (CTile* Tile : Tiles)
			Tile->Destroy();
		Tiles.clear();

		for (CActor* Actor : TileMapActors)
			Actor->Destroy();
		TileMapActors.clear();
	}
public:
	inline const static size_t TileNone = -1;

private:
	std::vector<CTile*> Tiles;

	size_t TileWidth = 0;
	size_t TileHeight = 0;
	size_t TileMapRow = 0;
	size_t TileMapCol = 0;

	std::vector<CActor*> TileMapActors;

};

