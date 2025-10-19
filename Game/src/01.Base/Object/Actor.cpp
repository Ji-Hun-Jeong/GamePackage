#include "pch.h"
#include "Actor.h"
#include "../World/World.h"

void CActor::Initalize()
{
	Transform = GetWorld()->NewObject<CTransform>();
}

void CActor::SetRenderComponent()
{
	RenderComponent = GetWorld()->NewObject<CRenderComponent>();

	RenderComponent->SetImageChangeEvent([&](const Vector3& InImageScale)->void
		{
			Transform->SetOffsetScale(InImageScale);
		});
	RenderComponent->AddVertexConstBuffer(sizeof(Transform->GetModelMatrix()));
}

void CActor::SetInteractionComponent()
{
	InteractionComponent = GetWorld()->NewObject<CInteractionComponent>();
}

void CActor::SetAnimator()
{
	assert(RenderComponent);
	Animator = GetWorld()->NewObject<CAnimator>();
}
