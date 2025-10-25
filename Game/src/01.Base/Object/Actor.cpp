#include "pch.h"
#include "Actor.h"
#include "../World/World.h"

void CActor::Initalize()
{
	Transform = NewObject<CTransform>(this);
}

void CActor::Destroy()
{
	GetWorld()->PushWorldSynchronizeEvent([this]()->void
		{
			GetWorld()->MarkDestroyed();

			bDestroy = true;
		});
	for (auto& Child : Childs)
		Child->Destroy();
	for (auto& Component : Components)
		Component->Destroy();
}

void CActor::SetRenderComponent()
{
	RenderComponent = NewObject<CRenderComponent>(this);

	RenderComponent->AddImageChangeEvent([this](const Vector2& InImageSize)->void
		{
			Transform->SetScale(Vector3(InImageSize.x, InImageSize.y, Transform->GetScale().z));
		});
	RenderComponent->SetMesh(0);
	RenderComponent->SetPSO(EPSOType::Basic);
	RenderComponent->AddVertexConstBuffer(sizeof(Matrix));
}

void CActor::SetInteractionComponent()
{
	InteractionComponent = NewObject<CInteractionComponent>(this);
}

void CActor::SetAnimator()
{
	assert(RenderComponent);
	Animator = NewObject<CAnimator>(this);
}
