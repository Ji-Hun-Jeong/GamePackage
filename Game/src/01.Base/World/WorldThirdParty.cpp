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
		WorldActor->RenderActor(InRenderer);
	}
	InRenderer.Draw();
}

void CWorld::CollectMouseInteraction(CMouseInteractionManager& InMouseInteractionManager)
{
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
