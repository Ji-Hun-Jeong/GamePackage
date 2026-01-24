#include "pch.h"
#include "WzLoader.h"
#include "01.Base/Actor/Actor.h"

#include "04.Renderer/RenderResourceLoader.h"
Vector3 CWzUtils::GetWorldPositionFromOrigin(Vector2 InImageScale, Vector2 InWzOrigin)
{
	float FinalX = (InImageScale.x / 2.0f) - InWzOrigin.x;
	float FinalY = (InImageScale.y / 2.0f) - InWzOrigin.y;

	return Vector3(FinalX, -FinalY, 0.0f);
}

Vector3 CWzUtils::GetWorldPositionFromOrigin(const std::wstring& InImagePath, Vector2 InWzOrigin)
{
	CImage* Image = CRenderResourceLoader::GetInst().GetImage(InImagePath);
	return GetWorldPositionFromOrigin(Image->GetTexture2D().GetTextureSize(), InWzOrigin);
}

Vector3 CWzUtils::GetWorldPositionFromOrigin(const CSpriteRenderComponent& InSpriteRenderComponent, Vector2 InWzOrigin)
{
	Vector2 ImageSize = InSpriteRenderComponent.GetImageScale();
	return GetWorldPositionFromOrigin(ImageSize, InWzOrigin);
}

bool CWzUtils::SetWorldPositionFromOrigin(CActor& InActor, Vector2 InWzOrigin)
{
	if (CSpriteRenderComponent* RC = InActor.GetSpriteRenderComponent())
	{
		Vector3 WorldPosition = GetWorldPositionFromOrigin(*RC, InWzOrigin);
		InActor.GetTransform()->SetPosition(WorldPosition);
		return true;
	}
	return false;
}