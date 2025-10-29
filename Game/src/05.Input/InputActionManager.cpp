#include "pch.h"
#include "InputActionManager.h"
//#include "01.Base/Object/Object.h"
//
//class CCallSetInputAction : public INewObjectEvent
//{
//public:
//	CCallSetInputAction(CInputActionManager& InInputActionManager)
//		: InputActionManager(InInputActionManager)
//	{}
//private:
//	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
//	{
//		InNewObject.SetInputAction(InputActionManager);
//	}
//	CInputActionManager& InputActionManager;
//};
void CInputActionManager::InitalizeFromWorld(CWorld& InWorld)
{
	//InWorld.AddNewObjectEvent(std::make_unique<CCallSetInputAction>(*this));

}

