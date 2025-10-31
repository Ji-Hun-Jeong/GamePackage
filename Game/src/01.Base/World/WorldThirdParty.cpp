#include "pch.h"
#include "World.h"
#include "04.Renderer/SpriteRenderer.h"
#include "05.Input/InputActionManager.h"

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
		if (WorldActor->GetTransform()->OnVariation())
		{
			for (auto& Child : WorldActor->GetChild())
				Child->GetTransform()->SetVariation(true);

			WorldActor->GetTransform()->SetVariation(false);
		}

		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent == nullptr)
			continue;

		// SetupResource
		RenderComponent->SetupPSOToRSO(PSOManager);
		RenderComponent->SetupResourceToRSO(RenderResourceLoader);
		RenderComponent->SetupMappingInstanceToRSO(RenderResourceLoader);

		WorldActor->CaptureSnapShot(ScreenWidth, ScreenHeight);
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

	InRenderer.UpdateRSOs(RenderStateObjects);
	InRenderer.RenderRSOs(RenderStateObjects);
}