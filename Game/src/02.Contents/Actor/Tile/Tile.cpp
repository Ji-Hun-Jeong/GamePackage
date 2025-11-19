#include "pch.h"
#include "Tile.h"
#include "GameCore.h"

void CTile::MoveActor(ETilePositionType InTilePositionType)
{
	if (PutOnActor == nullptr)
		PutOnActor = GetWorld()->SpawnActor<CStaticActor>(this);

	Vector2 TileScale = Vector2(Transform->GetScale().x, Transform->GetScale().y);
	const Vector2& ImageScale = PutOnActor->GetSpriteRenderComponent()->GetImageScale();
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
		PutOnActor->GetTransform()->SetPosition(Vector3(Position.x, Position.y, 0.0f));
		break;
	case ETilePositionType::Left:
		PutOnActor->GetTransform()->SetPosition(Vector3(Left.x, Left.y, 0.0f));
		break;
	case ETilePositionType::Right:
		PutOnActor->GetTransform()->SetPosition(Vector3(Right.x, Left.y, 0.0f));
		break;
	case ETilePositionType::Top:
		PutOnActor->GetTransform()->SetPosition(Vector3(Top.x, Top.y, 0.0f));
		break;
	case ETilePositionType::Bottom:
		PutOnActor->GetTransform()->SetPosition(Vector3(Bottom.x, Bottom.y, 0.0f));
		break;
	case ETilePositionType::LeftTop:
		PutOnActor->GetTransform()->SetPosition(Vector3(Left.x- ImageScale.x/2.0f, Top.y, 0.0f));
		break;
	case ETilePositionType::LeftBottom:
		PutOnActor->GetTransform()->SetPosition(Vector3(Left.x - ImageScale.x / 2.0f, Bottom.y, 0.0f));
		break;
	case ETilePositionType::RightTop:
		PutOnActor->GetTransform()->SetPosition(Vector3(Right.x + ImageScale.x / 2.0f, Top.y, 0.0f));
		break;
	case ETilePositionType::RightBottom:
		PutOnActor->GetTransform()->SetPosition(Vector3(Right.x + ImageScale.x / 2.0f, Bottom.y, 0.0f));
		break;
	default:
		assert(0);
		break;
	}
}
