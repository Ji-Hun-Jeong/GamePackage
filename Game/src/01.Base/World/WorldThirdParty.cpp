#include "pch.h"
#include "World.h"
#include "00.App/Screen.h"
#include "01.Base/Actor/Camera.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderSorter.h"
#include "04.Renderer/ImGuiManager.h"
#include "06.Interaction/MouseInteractionManager.h"
#include "07.Collision/PixelCollisionManager.h"

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent)
			RenderComponent->Render(InRenderer);
	}
	InRenderer.Draw();
}

void CWorld::CollectMouseInteraction(CMouseInteractionManager& InMouseInteractionManager)
{
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
		CImGuiManager::GetInst().SetOffSet(Camera->GetTransform()->GetFinalPosition2D());
	}
	InMouseInteractionManager.SetMouseWorldPosition(MousePosition);

	for (auto& WorldActor : WorldActors)
	{
		CInteractionComponent* InteractionComponent = WorldActor->GetInteractionComponent();
		if (InteractionComponent)
			InteractionComponent->InteractionCheck(InMouseInteractionManager);
	}
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
		std::vector<CCollider*> Colliders = WorldActor->GetComponents<CCollider>();
		for (auto Collider : Colliders)
			InCollisionManager.RequestCollision(*Collider);
	}
	InCollisionManager.CheckCollisionProcess();
}

void CWorld::SetScreen(CScreen& InScreen)
{
	const CCamera* Camera = nullptr;
	for (auto WorldActor : WorldActors)
	{
		if (WorldActor->GetType() == CCamera::GetStaticType())
		{
			Camera = static_cast<const CCamera*>(WorldActor);
			InScreen.SetScreenPosition(Camera->GetTransform()->GetFinalPosition2D());
			break;
		}
	}
}
