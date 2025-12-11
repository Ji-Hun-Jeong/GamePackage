#include "pch.h"
#include "Actor.h"

#include "04.Renderer/SpriteRenderer.h"

CActor::CActor()
	: Owner(nullptr)
	, bActive(true)
	, Transform(nullptr)
{
	Transform = AddComponent<CTransform>();
}

void CActor::RenderActor(CSpriteRenderer& InRenderer)
{
	if (RenderComponent)
	{
		const Vector3& Position = Transform->GetFinalPosition();
		const Vector3& Rotation = Transform->GetRotation();
		const Vector3& Scale = Transform->GetScale();

		RenderComponent->Render(InRenderer, Position, Rotation, Scale);
	}
	for (auto Collider : Colliders)
		Collider->DebugRender(InRenderer);
}
