#include "pch.h"
#include "MouseManager.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/Component/InteractionComponent.h"

class CSetMouseInteracter : public INewObjectEvent
{
public:
	CSetMouseInteracter(CMouseManager& InMouseManager)
		: MouseManager(InMouseManager)
	{}

private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		CInteractionComponent& InteractionComponent = static_cast<CInteractionComponent&>(InNewObject);
		InteractionComponent.SetMouseInteracter(MouseManager.NewMouseInteracter());
	}
	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectTypeEvent(CInteractionComponent::GetStaticType(), std::make_unique<CSetMouseInteracter>(*this));
}
