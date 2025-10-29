#include "pch.h"
#include "Actor.h"
#include "../World/World.h"

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