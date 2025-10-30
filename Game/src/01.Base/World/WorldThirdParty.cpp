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

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	const CPSOManager& PSOManager = InRenderer.GetPSOManager();
	CRenderResourceLoader& RenderResourceLoader = InRenderer.GetRenderResourceLoader();
	const uint32_t ScreenWidth = InRenderer.GetScreenWidth();
	const uint32_t ScreenHeight = InRenderer.GetScreenHeight();

	std::vector<CRenderStateObject*> RenderStateObjects;
	RenderStateObjects.reserve(WorldActors.size());

	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent == nullptr)
			continue;

		// SetupResource
		RenderComponent->SetupPSOToRSO(PSOManager);
		RenderComponent->SetupResourceToRSO(RenderResourceLoader);
		RenderComponent->SetupMappingInstanceToRSO(RenderResourceLoader);

		CTransform* Transform = WorldActor->GetTransform();
		RenderComponent->SynchronizeScaleToImageOnImageChange(*Transform);

		if (Transform->OnVariation())
		{
			RenderComponent->UpdateModelDataToNDC(*Transform, ScreenWidth, ScreenHeight);
			Transform->SetVariation(false);
		}

		RenderComponent->MapUpdatedBuffersToRSO();

		RenderComponent->ClearState();
		// PushRSO
		CRenderStateObject* RenderStateObject = RenderComponent->GetRenderStateObject();
		RenderStateObjects.push_back(RenderStateObject);
	}

	InRenderer.UpdateRSOs(RenderStateObjects);
	InRenderer.RenderRSOs(RenderStateObjects);
}