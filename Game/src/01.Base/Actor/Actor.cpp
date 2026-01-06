#include "pch.h"
#include "Actor.h"

#include "04.Renderer/SpriteRenderer.h"
#include "Component/RigidBody.h"
#include "Component/Animation/Animator.h"

CActor::CActor()
	: Owner(nullptr)
	, bActive(true)
	, Transform(nullptr)
{
	Transform = AddComponent<CTransform>();
}

void CActor::AttachComponent(CComponent* InComponent)
{
	InComponent->OwnerActor = this;
	Components.push_back(InComponent);
	if (CCollider* Collider = dynamic_cast<CCollider*>(InComponent))
		Colliders.push_back(Collider);
	else if (CSpriteRenderComponent* Component = dynamic_cast<CSpriteRenderComponent*>(InComponent))
		SpriteRenderComponent = Component;
	else if (CRigidBody* Component = dynamic_cast<CRigidBody*>(InComponent))
		RigidBody = Component;
	else if (CAnimator* Component = dynamic_cast<CAnimator*>(InComponent))
		Animator = Component;
}

void CActor::DetachComponent(CComponent* InComponent)
{
	for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
	{
		if (*Iter == InComponent)
		{
			DestroyObject(*Iter);
			Components.erase(Iter);
			break;
		}
	}
	if (CCollider* Collider = dynamic_cast<CCollider*>(InComponent))
	{
		for (auto Iter = Colliders.begin(); Iter != Colliders.end(); ++Iter)
		{
			if (*Iter == Collider)
			{
				Colliders.erase(Iter);
				break;
			}
		}
	}
	else if (CSpriteRenderComponent* Component = dynamic_cast<CSpriteRenderComponent*>(InComponent))
		SpriteRenderComponent = nullptr;
	else if (CRigidBody* Component = dynamic_cast<CRigidBody*>(InComponent))
		RigidBody = nullptr;
	else if (CAnimator* Component = dynamic_cast<CAnimator*>(InComponent))
		Animator = nullptr;
}

void CActor::Update(float InDeltaTime)
{
	// 입력, 이동 로직
	if (RigidBody)
		RigidBody->Update(*Transform, InDeltaTime);
	if (Animator)
		Animator->PlayAnimation(InDeltaTime);
}

void CActor::RenderActor(CSpriteRenderer& InRenderer)
{
	if (SpriteRenderComponent)
	{
		const Vector3& Position = Transform->GetWorldPosition();
		const Vector3& Rotation = Transform->GetRotation();
		const Vector3& Scale = Transform->GetScale();

		SpriteRenderComponent->Render(InRenderer, Position, Rotation, Scale);
	}
	for (auto Collider : Colliders)
		Collider->DebugRender(InRenderer);
}
