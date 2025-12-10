#pragma once
#include "01.Base/Actor/StaticActor.h"

class CTransformUtils
{
public:
	static bool GetTopPosition(CStaticActor& InPivotActor, CStaticActor& InOffsetActor, Vector3* OutResultPosition)
	{
		if (InPivotActor.GetSpriteRenderComponent()->IsImageType() == false || InOffsetActor.GetSpriteRenderComponent()->IsImageType() == false)
			return false;

		Vector2 PivotImageScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		Vector2 OffsetImageScale = InOffsetActor.GetSpriteRenderComponent()->GetImageScale();
		const Vector3& PivotActorOriginPosition = InPivotActor.GetTransform()->GetPosition();
		*OutResultPosition = Vector3(PivotActorOriginPosition.x
			, PivotActorOriginPosition.y + PivotImageScale.y / 2.0f + OffsetImageScale.y / 2.0f, PivotActorOriginPosition.z);
		return true;
	}
	static bool GetBottomPosition(CStaticActor& InPivotActor, CStaticActor& InOffsetActor, Vector3* OutResultPosition)
	{
		if (InPivotActor.GetSpriteRenderComponent()->IsImageType() == false || InOffsetActor.GetSpriteRenderComponent()->IsImageType() == false)
			return false;

		Vector2 PivotImageScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		Vector2 OffsetImageScale = InOffsetActor.GetSpriteRenderComponent()->GetImageScale();
		const Vector3& PivotActorOriginPosition = InPivotActor.GetTransform()->GetPosition();
		*OutResultPosition = Vector3(PivotActorOriginPosition.x
			, PivotActorOriginPosition.y - (PivotImageScale.y / 2.0f + OffsetImageScale.y / 2.0f), PivotActorOriginPosition.z);
		return true;
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
};

