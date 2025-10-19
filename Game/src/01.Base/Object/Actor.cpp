#include "pch.h"
#include "Actor.h"
#include "../World/World.h"

void CActor::Initalize()
{
	Transform = GetWorld()->NewObject<CTransform>(this);
}

void CActor::Destroy()
{
	GetWorld()->PushWorldSynchronizeEvent([this]()->void
		{
			GetWorld()->MarkDestroyed();

			bDestroy = true;

			EndPlay();
		});
	for (auto& Child : Childs)
		Child->Destroy();
	for (auto& Component : Components)
		Component->Destroy();
}

void CActor::SetRenderComponent()
{
	RenderComponent = GetWorld()->NewObject<CRenderComponent>(this);

	RenderComponent->SetImageChangeEvent([&](const Vector3& InImageScale)->void
		{
			Transform->SetOffsetScale(InImageScale);
		});
	RenderComponent->AddVertexConstBuffer(sizeof(Transform->GetModelMatrix()));
}

void CActor::SetInteractionComponent()
{
	InteractionComponent = GetWorld()->NewObject<CInteractionComponent>(this);
}

void CActor::SetAnimator()
{
	assert(RenderComponent);
	Animator = GetWorld()->NewObject<CAnimator>(this);
}
