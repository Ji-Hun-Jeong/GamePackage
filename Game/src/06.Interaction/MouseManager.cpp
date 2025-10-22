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
			InteractionComponent.SetMouseInteracter(MouseManager.NewMouseInteracter());
		}
		if (CMousePointer::GetStaticType() == InNewObject.GetType())
		{
			CMousePointer* MousePointer = static_cast<CMousePointer*>(&InNewObject);
			MouseManager.MousePosition = std::make_unique<CMousePosition>();

			MousePointer->SetMousePosition(MouseManager.MousePosition.get());
			MousePointer->AddDestroyEvent([this, MousePointer](CObject& InObject)->void
				{
					ResetMousePosition(*MousePointer);
				});
		}
	}
	void ResetMousePosition(CMousePointer& InMousePointer)
	{
		if (InMousePointer.GetMousePosition() == MouseManager.MousePosition.get())
			MouseManager.MousePosition = nullptr;
	}
	CMouseManager& MouseManager;
};
void CMouseManager::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectEvent(std::make_unique<CSetMouseInteracter>(*this));
}