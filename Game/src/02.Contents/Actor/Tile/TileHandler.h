#pragma once
#include "TileMap.h"

class CTileFocus : public CStaticActor
{
	GENERATE_OBJECT(CTileFocus)
public:
	CTileFocus()
	{
		SetLineActor();
		MarkColor = Vector3(0.0f, 1.0f, 1.0f);
	}
	~CTileFocus() = default;

public:
	void SetMarkColor(const Vector3& InMarkColor)
	{
		MarkColor = InMarkColor;
	}
	void SetFocusTile(CTile* InTile)
	{
		if (InTile)
		{
			Activate(true);
			Transform->SetPosition(InTile->GetTransform()->GetFinalPosition());
			Transform->SetScale(InTile->GetTransform()->GetScale());
			SpriteRenderComponent->SetColor(MarkColor, 1.0f);
			SpriteRenderComponent->SetLayer(InTile->GetSpriteRenderComponent()->GetLayer() + 1);
		}
		else
			Activate(false);
	}
	CTile* GetFocusTile() const { return CurrentFocusTile; }

private:
	CTile* CurrentFocusTile = nullptr;

	Vector3 MarkColor;
};

class CTileMapper
{
public:
	CTileMapper() = default;
	~CTileMapper() = default;

public:
	void Map(CTile& InTile, CStaticActor& InMappingActor)
	{
		MappingContexts[&InTile] = &InMappingActor;
	}
	CStaticActor& UnMap(CTile& InTile)
	{
		CStaticActor* MappingActor = MappingContexts.find(&InTile)->second;
		MappingContexts.erase(&InTile);
		return *MappingActor;
	}
	bool IsAlreadyMapping(CTile& InTile)
	{
		return MappingContexts.contains(&InTile);
	}
	CStaticActor* GetMappingActor(CTile& InTile)
	{
		auto Iter = MappingContexts.find(&InTile);
		if (Iter == MappingContexts.end())
			return nullptr;
		return Iter->second;
	}

private:
	std::map<CTile*, CStaticActor*> MappingContexts;

};

enum class ETilePositionType
{
	Center,
	Left,
	Right,
	Top,
	Bottom,
	LeftTop,
	LeftBottom,
	RightTop,
	RightBottom,
	None,
};

class CTileMover
{
public:
	CTileMover() = default;
	~CTileMover() = default;

public:
	static void MoveActor(CTile& InTile, CStaticActor& InMovedActor, ETilePositionType InTilePositionType)
	{
		Vector2 TileScale = Vector2(InTile.GetTransform()->GetScale().x, InTile.GetTransform()->GetScale().y);
		const Vector2& ImageScale = InMovedActor.GetSpriteRenderComponent()->GetImageScale();
		Vector2 OffsetScale = TileScale - ImageScale;

		Vector2 Position = InTile.GetTransform()->GetFinalPosition2D();
		Vector2 Scale = InTile.GetTransform()->GetScale2D();

		const Vector2 Top = Vector2(Position.x, Position.y + OffsetScale.y / 2.0f);
		const Vector2 Bottom = Vector2(Position.x, Position.y - OffsetScale.y / 2.0f);
		const Vector2 Left = Vector2(Position.x - OffsetScale.x / 2.0f, Position.y);
		const Vector2 Right = Vector2(Position.x + OffsetScale.x / 2.0f, Position.y);

		switch (InTilePositionType)
		{
		case ETilePositionType::Center:
			InMovedActor.GetTransform()->SetPosition(Vector3(Position.x, Position.y, 0.0f));
			break;
		case ETilePositionType::Left:
			InMovedActor.GetTransform()->SetPosition(Vector3(Left.x, Left.y, 0.0f));
			break;
		case ETilePositionType::Right:
			InMovedActor.GetTransform()->SetPosition(Vector3(Right.x, Left.y, 0.0f));
			break;
		case ETilePositionType::Top:
			InMovedActor.GetTransform()->SetPosition(Vector3(Top.x, Top.y, 0.0f));
			break;
		case ETilePositionType::Bottom:
			InMovedActor.GetTransform()->SetPosition(Vector3(Bottom.x, Bottom.y, 0.0f));
			break;
		case ETilePositionType::LeftTop:
			InMovedActor.GetTransform()->SetPosition(Vector3(Left.x - ImageScale.x / 2.0f, Top.y, 0.0f));
			break;
		case ETilePositionType::LeftBottom:
			InMovedActor.GetTransform()->SetPosition(Vector3(Left.x - ImageScale.x / 2.0f, Bottom.y, 0.0f));
			break;
		case ETilePositionType::RightTop:
			InMovedActor.GetTransform()->SetPosition(Vector3(Right.x + ImageScale.x / 2.0f, Top.y, 0.0f));
			break;
		case ETilePositionType::RightBottom:
			InMovedActor.GetTransform()->SetPosition(Vector3(Right.x + ImageScale.x / 2.0f, Bottom.y, 0.0f));
			break;
		default:
			assert(0);
			break;
		}
	}
private:


};

class CTileHandler : public CActor
{
	GENERATE_OBJECT(CTileHandler)
public:
	CTileHandler() = default;
	~CTileHandler() = default;

public:
	void HandleTile(CTile& InHandledTile, uint32_t InLayer);
	void ClearHandledTiles()
	{
		HandledTiles.clear();
		for (auto TileMarker : TileMarkers)
			TileMarker->Destroy();
		TileMarkers.clear();
	}

	bool IsExist(const CTile& InHandledTile) const
	{
		for (CTile* HandledTile : HandledTiles)
		{
			if (HandledTile == &InHandledTile)
				return true;
		}
		return false;
	}
	uint32_t GetMarkerLayer() const { return HandledTiles[0]->GetSpriteRenderComponent()->GetLayer(); }
	bool IsEmpty() const { return HandledTiles.empty(); }
	Vector3 GetCenterPosition() const;

	void MoveActorByTile(ETilePositionType TilePositionType, CTileMapper& TileMapper)
	{
		for (auto HandledTile : HandledTiles)
		{
			CStaticActor* MappingActor = TileMapper.GetMappingActor(*HandledTile);
			if (MappingActor == nullptr)
				return;
		}

		for (auto HandledTile : HandledTiles)
		{
			CStaticActor* MappingActor = TileMapper.GetMappingActor(*HandledTile);
			CTileMover::MoveActor(*HandledTile, *MappingActor, TilePositionType);
		}

	}

private:
	std::vector<CTile*> HandledTiles;

	std::vector<CStaticActor*> TileMarkers;

};