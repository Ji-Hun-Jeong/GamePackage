#include "pch.h"
#include "Renderer.h"
#include "01.Base/World/World.h"

class CGetRenderStateObject : public INewObjectEvent
{
public:
	CGetRenderStateObject(CRenderer& InRenderer)
		: Renderer(InRenderer)
	{}
private:
	// INewObjectEvent을(를) 통해 상속됨
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		if (CRenderComponent::GetStaticType() != InNewObject.GetType())
			return;

		CRenderComponent* RenderComponent = static_cast<CRenderComponent*>(&InNewObject);
		RenderComponent->SetRenderStateObjectEvent = [this](CRenderStateObject* InRenderStateObject)
			{
				Renderer.AddRenderStateObject(std::unique_ptr<CRenderStateObject>(InRenderStateObject));
			};
	}
	CRenderer& Renderer;

};
void CRenderer::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CGetRenderStateObject>(*this));
}
