#include "pch.h"
#include "MouseManager.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/Component/InteractionComponent.h"
#include "02.Contents/Actor/Base/MousePointer.h"

class CSetMouseInteracter : public INewObjectEvent
{
public:
	CSetMouseInteracter(CMouseManager& InMouseManager)
		: MouseManager(InMouseManager)
	{}

private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		if (CInteractionComponent::GetStaticType() == InNewObject.GetType())
		{
			CInteractionComponent& InteractionComponent = static_cast<CInteractionComponent&>(InNewObject);
			InteractionComponent.SetMouseManager(&MouseManager);
		}
		if (CMousePositionComponent::GetStaticType() == InNewObject.GetType())
		{
			CMousePositionComponent* MousePositionComponent = static_cast<CMousePositionComponent*>(&InNewObject);
			MousePositionComponent->SetMouseManager(MouseManager);
		}
	}

	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CSetMouseInteracter>(*this));
}