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
			
			GetMousePointer = [this](CObject& InObject)->void
				{
					CMousePointer* MousePointer = static_cast<CMousePointer*>(&InObject);
					MouseManager.SetMousePositionInstance(MousePointer->GetMousePositionInstance());
				};
			ReleaseMousePointer = [this](CObject& InObject)->void
				{
					CMousePointer* MousePointer = static_cast<CMousePointer*>(&InObject);
					MouseManager.SetMousePositionInstance(nullptr);
				};
			MousePointer->AddBeginEvent(GetMousePointer);
			MousePointer->AddEndEvent(ReleaseMousePointer);
		}
	}
	std::function<void(CObject&)> GetMousePointer;
	std::function<void(CObject&)> ReleaseMousePointer;

	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CSetMouseInteracter>(*this));
}