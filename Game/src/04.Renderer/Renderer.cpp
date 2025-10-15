#include "pch.h"
#include "Renderer.h"
#include "01.Base/World/World.h"

class CGetRenderStateObject : public INewObjectEvent
{
public:
	CGetRenderStateObject(CRenderer& InRenderer, CRenderResourceLoader& InRenderResourceLoader)
		: Renderer(InRenderer)
		, RenderResourceLoader(InRenderResourceLoader)
	{}
private:
	// INewObjectEvent을(를) 통해 상속됨
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		CRenderComponent* RenderComponent = static_cast<CRenderComponent*>(&InNewObject);
		CRenderStateObject* RenderStateObject = Renderer.NewRenderStateObject();

		RenderComponent->SetRenderStateObject(RenderStateObject);
		RenderComponent->SetRenderResourceLoader(&RenderResourceLoader);
	}

	CRenderer& Renderer;
	CRenderResourceLoader& RenderResourceLoader;
};
void CRenderer::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(CRenderComponent::GetStaticType(), std::make_unique<CGetRenderStateObject>(*this, RenderResourceLoader));
}
