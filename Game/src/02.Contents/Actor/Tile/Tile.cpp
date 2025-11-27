#include "pch.h"
#include "Tile.h"
#include "GameCore.h"

void CTile::MoveActor(CStaticActor& InMovedActor, ETilePositionType InTilePositionType)
{
	/*if (PutOnActor == nullptr)
		PutOnActor = GetWorld()->SpawnActor<CStaticActor>(this);*/

	Vector2 TileScale = Vector2(Transform->GetScale().x, Transform->GetScale().y);
	const Vector2& ImageScale = InMovedActor.GetSpriteRenderComponent()->GetImageScale();
	Vector2 OffsetScale = TileScale - ImageScale;

	Vector2 Position = Transform->GetFinalPosition2D();
	Vector2 Scale = Transform->GetScale2D();

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
