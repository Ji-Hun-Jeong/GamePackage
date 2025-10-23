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
		if (CMousePointer::GetStaticType() == InNewObject.GetType())
		{
			CMousePointer* MousePointer = static_cast<CMousePointer*>(&InNewObject);
			MousePointer->SetMouseManager(MouseManager);
		}
	}

	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CSetMouseInteracter>(*this));
}