#include "pch.h"
#include "World.h"
#include "01.Base/Actor/Camera.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderSorter.h"
#include "05.Input/InputActionManager.h"
#include "06.Interaction/MouseInteractionManager.h"
#include "07.Collision/PixelCollisionManager.h"

void CWorld::PerformInputAction(CInputActionManager& InInputActionManager)
{
	CInputActionValueCollector Collector;
	for (auto& WorldActor : WorldActors)
		WorldActor->SetupInputActionValue(Collector);

	InInputActionManager.PerformAction(Collector);
}
void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	std::vector<CRenderStateObject*> RenderStateObjects;
	RenderStateObjects.reserve(WorldActors.size());

	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent)
			RenderComponent->Render(InRenderer);
	}
	InRenderer.Draw();
}

void CWorld::DetectMouseInteraction(CMouseInteractionManager& InMouseInteractionManager)
{
	std::vector<CMouseInteracter*> MouseInteracters;
	Vector2 MousePosition = CMouseManager::GetInst().GetMousePosition();

	const CCamera* Camera = nullptr;
	for (auto WorldActor : WorldActors)
	{
		if (WorldActor->GetType() != CCamera::GetStaticType())
			continue;
		Camera = static_cast<const CCamera*>(WorldActor);
	}

	if (Camera)
	{
		MousePosition.x += int32_t(Camera->GetTransform()->GetFinalPosition().x);
		MousePosition.y += int32_t(Camera->GetTransform()->GetFinalPosition().y);
	}

	for (auto& WorldActor : WorldActors)
	{
		CInteractionComponent* InteractionComponent = WorldActor->GetInteractionComponent();
		if (InteractionComponent)
			MouseInteracters.push_back(InteractionComponent->GetMouseInteracter());
	}

	InMouseInteractionManager.FindFocusInteracter(std::move(MouseInteracters), MousePosition);
}

void CWorld::CollectCollisionObjects(CPixelCollisionManager& InPixelCollisionManager)
{
	/*for (auto& WorldActor : WorldActors)
	{
		CPixelCollider* PixelCollider = WorldActor->GetPixelCollider();
		if (PixelCollider)
			PixelCollider->CollisionProcess(InPixelCollisionManager);
	}*/
}

void CWorld::ProgressCollisionCheck(CCollisionManager& InCollisionManager)
{
	for (auto WorldActor : WorldActors)
	{
		std::vector<CRectCollider*> RectColliders = WorldActor->GetComponents<CRectCollider>();
		for (auto RectCollider : RectColliders)
			InCollisionManager.RequestCollision(*RectCollider);

		std::vector<CCircleCollider*> CircleColliders = WorldActor->GetComponents<CCircleCollider>();
		for (auto CircleCollider : CircleColliders)
			InCollisionManager.RequestCollision(*CircleCollider);
	}
	InCollisionManager.CheckCollisionProcess();
}
