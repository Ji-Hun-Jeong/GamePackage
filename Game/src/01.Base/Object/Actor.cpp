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

	Graphics::TBufferDesc VertexConstBufferDesc;
	VertexConstBufferDesc.ByteWidth = sizeof(Transform->GetModelMatrix());
	VertexConstBufferDesc.Usage = Graphics::EUsage::UsageDynamic;
	VertexConstBufferDesc.BindFlags = Graphics::EBindFlags::BindConstantBuffer;
	VertexConstBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;

	RenderComponent->AddVertexConstBuffer(VertexConstBufferDesc);
}