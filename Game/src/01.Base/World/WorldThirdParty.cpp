#include "pch.h"
#include "World.h"
#include "04.Renderer/SpriteRenderer.h"

void CWorld::RenderWorld(CSpriteRenderer& InRenderer)
{
	const CPSOManager& PSOManager = InRenderer.GetPSOManager();
	CRenderResourceLoader& RenderResourceLoader = InRenderer.GetRenderResourceLoader();

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