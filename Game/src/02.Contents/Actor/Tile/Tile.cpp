#include "pch.h"
#include "Tile.h"
#include "GameCore.h"

void CTile::PutOnActor(const std::wstring& InActorImagePath, ETilePutMode InTilePutMode)
{
	if (ManagingActor == nullptr)
		ManagingActor = GetWorld()->SpawnActor<CStaticActor>(this);
	ManagingActor->GetSpriteRenderComponent()->SetDiffuseImage(InActorImagePath);

	switch (InTilePutMode)
	{
	case ETilePutMode::Center:
		break;
	case ETilePutMode::Up:
		break;
	case ETilePutMode::Down:
		ManagingActor->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
		break;
	case ETilePutMode::Left:
		break;
	case ETilePutMode::Right:
		break;
	default:
		break;
	}
}

void CTile::RevertPutOn()
{
	if (ManagingActor == nullptr)
		return;

	ManagingActor->Destroy();
	ManagingActor = nullptr;
}
