#pragma once
#include "01.Base/Actor/StaticActor.h"

class CTransformUtils
{
public:
	static Vector3 GetTopPosition(CStaticActor& InPivotActor, CStaticActor& InOffsetActor)
	{
		Vector2 PivotScale;
		if (InPivotActor.GetSpriteRenderComponent()->IsImageType())
			PivotScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		else
			PivotScale = InPivotActor.GetTransform()->GetScale2D();
		Vector2 OffsetScale;
		if (InOffsetActor.GetSpriteRenderComponent()->IsImageType())
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
		if (InPivotActor.GetSpriteRenderComponent()->IsImageType())
			PivotScale = InPivotActor.GetSpriteRenderComponent()->GetImageScale();
		else
			PivotScale = InPivotActor.GetTransform()->GetScale2D();
		Vector2 OffsetScale;
		if (InOffsetActor.GetSpriteRenderComponent()->IsImageType())
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
};

