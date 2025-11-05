#include "pch.h"
#include "World.h"
#include "01.Base/Actor/Camera.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderSorter.h"
#include "05.Input/InputActionManager.h"
#include "06.Interaction/MouseInteractionManager.h"

void CWorld::PerformInputAction(CInputActionManager& InInputActionManager)
{
	CInputActionValueCollector Collector;
	for (auto& WorldActor : WorldActors)
		WorldActor->SetupInputActionValue(Collector);

	InInputActionManager.PerformAction(Collector);
}

void CWorld::CaptureSnapShot(CSpriteRenderer& InRenderer)
{
	const CPSOManager& PSOManager = InRenderer.GetPSOManager();
	CRenderResourceLoader& RenderResourceLoader = InRenderer.GetRenderResourceLoader();

	const uint32_t ScreenWidth = InRenderer.GetScreenWidth();
	const uint32_t ScreenHeight = InRenderer.GetScreenHeight();
	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent)
		{
			// SetupResource
			RenderComponent->SetupPSOToRSO(PSOManager);
			RenderComponent->SetupResourceToRSO(RenderResourceLoader);
			RenderComponent->SetupMappingInstanceToRSO(RenderResourceLoader);
		}

		if (WorldActor->GetTransform()->OnVariation())
		{
			for (auto& Child : WorldActor->GetChild())
				Child->GetTransform()->SetVariation(true);
			if(RenderComponent)
				WorldActor->CaptureSnapShot(ScreenWidth, ScreenHeight);
			WorldActor->GetTransform()->SetVariation(false);
		}
	}
}

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	std::vector<CRenderStateObject*> RenderStateObjects;
	RenderStateObjects.reserve(WorldActors.size());

	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent == nullptr)
			continue;

		RenderComponent->MapUpdatedBuffersToRSO();

		RenderComponent->ClearState();
		// PushRSO
		CRenderStateObject* RenderStateObject = RenderComponent->GetRenderStateObject();
		RenderStateObjects.push_back(RenderStateObject);
	}

	CRenderSorter RenderSorter;
	RenderSorter.SortByLayer(RenderStateObjects);

	InRenderer.UpdateRSOs(RenderStateObjects);
	InRenderer.RenderRSOs(RenderStateObjects);
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
		{
			InteractionComponent->SetRectTransform(WorldActor->GetTransform()->GetFinalPosition().x, WorldActor->GetTransform()->GetFinalPosition().y
				, WorldActor->GetTransform()->GetScale().x, WorldActor->GetTransform()->GetScale().y);

			MouseInteracters.push_back(InteractionComponent->GetMouseInteracter());
		}
	}

	InMouseInteractionManager.FindFocusInteracter(std::move(MouseInteracters), MousePosition);
}
