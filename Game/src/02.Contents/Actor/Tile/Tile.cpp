#include "pch.h"
#include "Tile.h"
#include "GameCore.h"

//void CTile::PutOnActor(const std::wstring& InActorImagePath, ETilePutMode InTilePutMode)
//{
//	if (ManagingActor == nullptr)
//		ManagingActor = GetWorld()->SpawnActor<CStaticActor>(this);
//	ManagingActor->GetSpriteRenderComponent()->SetDiffuseImage(InActorImagePath);
//
//	Vector2 TileScale = Vector2(Transform->GetScale().x, Transform->GetScale().y);
//	const Vector2& ImageScale = ManagingActor->GetSpriteRenderComponent()->GetImageScale();
//	Vector2 OffsetScale = TileScale - ImageScale;
//
//	switch (InTilePutMode)
//	{
//	case ETilePutMode::Center:
//		break;
//	case ETilePutMode::Up:
//		ManagingActor->GetTransform()->SetPosition(Vector3(0.0f, OffsetScale.y / 2.0f, 0.0f));
//		break;
//	case ETilePutMode::Down:
//		ManagingActor->GetTransform()->SetPosition(Vector3(0.0f, -OffsetScale.y / 2.0f, 0.0f));
//		break;
//	case ETilePutMode::Left:
//		ManagingActor->GetTransform()->SetPosition(Vector3(-OffsetScale.x / 2.0f, 0.0f, 0.0f));
//		break;
//	case ETilePutMode::Right:
//		ManagingActor->GetTransform()->SetPosition(Vector3(OffsetScale.x / 2.0f, 0.0f, 0.0f));
//		break;
//	case ETilePutMode::RightBottom:
//		ManagingActor->GetTransform()->SetPosition(Vector3(TileScale.x / 2.0f, -OffsetScale.y / 2.0f, 0.0f));
//		break;
//	default:
//		break;
//	}
//}
//
//void CTile::RevertPutOn()
//{
//	if (ManagingActor == nullptr)
//		return;
//
//	ManagingActor->Destroy();
//	ManagingActor = nullptr;
//}
