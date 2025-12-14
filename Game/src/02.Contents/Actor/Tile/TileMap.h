#pragma once
#include "01.Base/Actor/Actor.h"
#include "02.Contents/Actor/UI/UI.h"
#include "03.Utils/CTransformUtils.h"
#include "04.Renderer/ImGuiManager.h"


class CTile : public CUI
{
	GENERATE_OBJECT(CTile)
public:
	CTile()
		: CUI()
	{
		static const Graphics::TMeshData& LineSquareMeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
		SpriteRenderComponent->SetLayer(1);
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
	CTileMap();
	~CTileMap() = default;

public:
	void LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol);
	CTile* GetTileByPosition(const Vector3& InPosition);
	void RenderTiles(bool bInRender)
	{
		for (auto Tile : Tiles)
			Tile->GetSpriteRenderComponent()->SetRender(bInRender);
	}
	void AttachToPanel(CUI& InMainPanel, std::function<void(CTile&)> InTileFocusEvent)
	{
		for (auto Tile : Tiles)
		{
			Tile->SetMouseFocusEvent([this, Tile, InTileFocusEvent]()->void
				{
					InTileFocusEvent(*Tile);
				});
			InMainPanel.AttachChildUI(*Tile);
		}
	}
	void DetachToPanel(CUI& InMainPanel)
	{
		for (auto Tile : Tiles)
		{
			Tile->SetMouseFocusEvent(nullptr);
			InMainPanel.DetachChildUI(*Tile);
		}
	}
	size_t GetTileMapRow() const { return TileMapRow; }
	size_t GetTileMapCol() const { return TileMapCol; }
	size_t GetTileWidth() const { return TileWidth; }
	size_t GetTileHeight() const { return TileHeight; }

private:
	std::vector<CTile*> Tiles;

	size_t TileWidth = 0;
	size_t TileHeight = 0;
	size_t TileMapRow = 0;
	size_t TileMapCol = 0;

};

