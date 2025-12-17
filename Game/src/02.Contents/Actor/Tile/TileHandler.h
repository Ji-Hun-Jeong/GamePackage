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
	void SetFocusTile(CTile* InTile, uint32_t InLayer)
	{
		if (InTile)
		{
			Activate(true);
			Transform->SetPosition(InTile->GetTransform()->GetFinalPosition());
			Transform->SetScale(InTile->GetTransform()->GetScale());
			SpriteRenderComponent->SetColor(MarkColor, 1.0f);
			SpriteRenderComponent->SetLayer(InLayer);
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
	void UnMap(CTile& InTile)
	{
		CStaticActor* MappingActor = MappingContexts.find(&InTile)->second;
		MappingContexts.erase(&InTile);
		MappingActor->Destroy();
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
		for (auto& Pair : TileMarkerMap)
			Pair.second->Destroy();
		TileMarkerMap.clear();
	}
	void EraseHandledTile(CTile& InTile)
	{
		for (auto Iter = TileMarkerMap.begin(); Iter != TileMarkerMap.end(); ++Iter)
		{
			CTile* Tile = Iter->first;
			CStaticActor* TileMarker = Iter->second;

			if (Tile == &InTile)
			{
				TileMarker->Destroy();
				TileMarkerMap.erase(Iter);
				break;
			}
		}
	}

	bool IsExist(const CTile& InHandledTile) const
	{
		for (auto& Pair : TileMarkerMap)
		{
			if (Pair.first == &InHandledTile)
				return true;
		}
		return false;
	}
	bool IsEmpty() const { return TileMarkerMap.empty(); }
	Vector3 GetCenterPosition() const;

	void MoveActorByTile(ETilePositionType TilePositionType, CTileMapper& TileMapper)
	{
		for (auto& Pair : TileMarkerMap)
		{
			CStaticActor* MappingActor = TileMapper.GetMappingActor(*Pair.first);
			if (MappingActor == nullptr)
				return;
		}

		for (auto& Pair : TileMarkerMap)
		{
			CStaticActor* MappingActor = TileMapper.GetMappingActor(*Pair.first);
			CTileMover::MoveActor(*Pair.first, *MappingActor, TilePositionType);
		}
	}

	void SetGroundByHandledTiles(const CTileMap& InTileMap, class CGroundManager& InGroundManager);

private:
	std::map<CTile*, CStaticActor*> TileMarkerMap;

};