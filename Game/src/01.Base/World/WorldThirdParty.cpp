#include "pch.h"
#include "World.h"
#include "04.Renderer/SpriteRenderer.h"

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	const CPSOManager& PSOManager = InRenderer.GetPSOManager();
	CRenderResourceLoader& RenderResourceLoader = InRenderer.GetRenderResourceLoader();
	const uint32_t ScreenWidth = InRenderer.GetScreenWidth();
	const uint32_t ScreenHeight = InRenderer.GetScreenHeight();


	std::vector<CRenderStateObject*> RenderStateObjects;
	RenderStateObjects.reserve(WorldActors.size());

	std::queue<TBufferMappingInstance> FinalUpdateBufferList;
	for (auto& WorldActor : WorldActors)
	{
		CRenderComponent* RenderComponent = WorldActor->GetRenderComponent();
		if (RenderComponent == nullptr)
			continue;

		// SetupResource
		RenderComponent->SetupPSO(PSOManager);
		RenderComponent->SetupResource(RenderResourceLoader);

		CTransform* Transform = WorldActor->GetTransform();
		if (Transform->OnVariation())
		{
			RenderComponent->UpdateModelVertexConstBufferData(*Transform, ScreenWidth, ScreenHeight);
			Transform->SetVariation(false);
		}

		// CollectUpdateBufferList
		std::queue<TBufferMappingInstance>& UpdateBufferList = RenderComponent->GetUpdateBufferList();
		while (UpdateBufferList.empty() == false)
		{
			FinalUpdateBufferList.push(UpdateBufferList.front());
			UpdateBufferList.pop();
		}

		// PushRSO
		CRenderStateObject* RenderStateObject = RenderComponent->GetRenderStateObject();
		RenderStateObjects.push_back(RenderStateObject);
	}

	InRenderer.UpdateRSOs(FinalUpdateBufferList);
	InRenderer.RenderRSOs(RenderStateObjects);
}