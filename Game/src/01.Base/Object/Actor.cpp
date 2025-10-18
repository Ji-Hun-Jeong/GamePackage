#include "pch.h"
#include "Actor.h"
#include "../World/World.h"

void CActor::Initalize()
{
	CObject::Initalize();

	Transform = GetWorld()->NewObject<CTransform>();

}

void CActor::SetRenderComponent()
{
	RenderComponent = GetWorld()->NewObject<CRenderComponent>();

	RenderComponent->AddVertexConstBuffer(sizeof(Transform->GetModelMatrix()));
}