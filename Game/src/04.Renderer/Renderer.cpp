#include "pch.h"
#include "Renderer.h"
#include "01.Base/World/World.h"

class CGetRenderStateObject : public IObjectEnterWorld
{
public:
	CGetRenderStateObject(CRenderer& InRenderer)
		: Renderer(InRenderer)
	{}
private:
	void EnterWorld(CWorld& InWorld, CObject& InEnterObject) override
	{
		CRenderComponent* RenderComponent = InEnterObject.GetRenderComponent();
		if (RenderComponent)
		{
			Renderer.AddRenderStateObject(std::unique_ptr<CRenderStateObject>(RenderComponent->GetRenderStateObject()));
			// Todo: Sort
		}
	}

	CRenderer& Renderer;

};
void CRenderer::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddObjectEnterWorldEvent(std::make_unique<CGetRenderStateObject>(*this));
}
