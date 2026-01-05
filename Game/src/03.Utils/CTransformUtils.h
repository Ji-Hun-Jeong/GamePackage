#pragma once
#include "01.Base/Actor/StaticActor.h"

class CTransformUtils
{
public:
	static Vector3 GetTopPosition(CStaticActor& InPivotActor, CStaticActor& InOffsetActor)
	{
		Vector2 PivotScale;
		if (InPivotActor.GetSpriteRenderComponent() && InPivotActor.GetSpriteRenderComponent()->IsImageType())
			PivotScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		else
			PivotScale = InPivotActor.GetTransform()->GetScale2D();
		Vector2 OffsetScale;
		if (InPivotActor.GetSpriteRenderComponent() && InOffsetActor.GetSpriteRenderComponent()->IsImageType())
			OffsetScale = InOffsetActor.GetSpriteRenderComponent()->GetImageScale();
		else
			OffsetScale = InOffsetActor.GetTransform()->GetScale2D();

		const Vector3& PivotActorOriginPosition = InPivotActor.GetTransform()->GetPosition();
		return Vector3(PivotActorOriginPosition.x
			, PivotActorOriginPosition.y + PivotScale.y / 2.0f + OffsetScale.y / 2.0f, PivotActorOriginPosition.z);
	}
	static Vector3 GetBottomPosition(CStaticActor& InPivotActor, CStaticActor& InOffsetActor)
	{
		Vector2 PivotScale;
		if (InPivotActor.GetSpriteRenderComponent() && InPivotActor.GetSpriteRenderComponent()->IsImageType())
			PivotScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		else
			PivotScale = InPivotActor.GetTransform()->GetScale2D();
		Vector2 OffsetScale;
		if (InPivotActor.GetSpriteRenderComponent() && InOffsetActor.GetSpriteRenderComponent()->IsImageType())
			OffsetScale = InOffsetActor.GetSpriteRenderComponent()->GetImageScale();
		else
			OffsetScale = InOffsetActor.GetTransform()->GetScale2D();

		const Vector3& PivotActorOriginPosition = InPivotActor.GetTransform()->GetPosition();
		return Vector3(PivotActorOriginPosition.x
			, PivotActorOriginPosition.y - (PivotScale.y / 2.0f + OffsetScale.y / 2.0f), PivotActorOriginPosition.z);
	}
	static bool IsPositionInsideActor(const Vector2& InPosition, CActor& InActor)
	{
		Vector2 TileCenter = Vector2(InActor.GetTransform()->GetFinalPosition().x, InActor.GetTransform()->GetFinalPosition().y);
		Vector2 TileScale = Vector2(InActor.GetTransform()->GetScale().x, InActor.GetTransform()->GetScale().y);

		float HalfWidth = TileScale.x * 0.5f;
		float HalfHeight = TileScale.y * 0.5f;

		float Left = TileCenter.x - HalfWidth;
		float Right = TileCenter.x + HalfWidth;
		float Top = TileCenter.y - HalfHeight;
		float Bottom = TileCenter.y + HalfHeight;

		if (InPosition.x >= Left && InPosition.x <= Right &&
			InPosition.y >= Top && InPosition.y <= Bottom)
		{
			return true;
		}
		return false;
	}
	static bool IsPositionInsideStaticActor(const Vector2& InPosition, CStaticActor& InActor)
	{
		Vector2 Center = Vector2(InActor.GetTransform()->GetFinalPosition().x, InActor.GetTransform()->GetFinalPosition().y);
		Vector2 Scale = InActor.GetSpriteRenderComponent()->GetImageScale();

		float HalfWidth = Scale.x * 0.5f;
		float HalfHeight = Scale.y * 0.5f;

		float Left = Center.x - HalfWidth;
		float Right = Center.x + HalfWidth;
		float Top = Center.y - HalfHeight;
		float Bottom = Center.y + HalfHeight;

		if (InPosition.x >= Left && InPosition.x <= Right &&
			InPosition.y >= Top && InPosition.y <= Bottom)
		{
			return true;
		}
		return false;
	}
};

