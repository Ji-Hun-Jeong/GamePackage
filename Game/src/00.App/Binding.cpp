#include "pch.h"
#include "Game.h"
#include "02.Contents/Actor/Base/MousePointer.h"

class CSetMouseEventToMouseManager : public Core::IMouseEvent
{
public:
	CSetMouseEventToMouseManager(CMouseManager& InMouseManager)
		: MouseManager(InMouseManager)
	{}
	
private:
	void OnActivate(EKeyType InKeyType, EButtonState InButtonState, const TMousePosition& InMousePosition) override
	{
		if (InButtonState == EButtonState::Tap)
			MouseManager.SetMouseClick(InKeyType, true);
		if (InButtonState == EButtonState::Released)
			MouseManager.SetMouseRelease(InKeyType, true);
	}
	CMouseManager& MouseManager;
};

class CSetMousePositionToActor : public Core::IMouseMove
{
public:
	CSetMousePositionToActor(CActor& InActor, CMouseManager& InMouseManager)
		: Actor(InActor), MouseManager(InMouseManager)
	{}
private:
	void MouseMove(int InX, int InY, uint32_t InScreenWidth, uint32_t InScreenHeight) override
	{
		float MouseX = InX - InScreenWidth / 2.0f;
		float MouseY = -InY + InScreenHeight / 2.0f;
		Actor.GetTransform()->SetPosition(Vector3(MouseX, MouseY, Actor.GetTransform()->GetPosition().z));
		MouseManager.SetMouseMove(true);
	}
	CActor& Actor;
	CMouseManager& MouseManager;
};

class CBindMousePointer : public INewObjectEvent
{
public:
	CBindMousePointer(Core::CWindow& InWindow, CMouseManager& InMouseManager)
		: Window(InWindow), MouseManager(InMouseManager)
	{}
private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		CMousePointer& MousePointer = static_cast<CMousePointer&>(InNewObject);
		Core::IMouseMove* SetMousePositionToActor = Window.RegistMouseMoveEvent(std::make_unique<CSetMousePositionToActor>(MousePointer, MouseManager));
		MousePointer.AddDestroyEvent([this, SetMousePositionToActor](CObject& InObject)->void
			{
				Window.DeRegistMouseMoveEvent(SetMousePositionToActor);
			});
	}
	Core::CWindow& Window;
	CMouseManager& MouseManager;
};

void CGame::Binding()
{
	InputManager.RegistMouseEvent(EKeyType::LButton, EButtonState::Tap, std::make_unique<CSetMouseEventToMouseManager>(MouseManager));
	InputManager.RegistMouseEvent(EKeyType::LButton, EButtonState::Released, std::make_unique<CSetMouseEventToMouseManager>(MouseManager));
	InputManager.RegistMouseEvent(EKeyType::RButton, EButtonState::Tap, std::make_unique<CSetMouseEventToMouseManager>(MouseManager));
	InputManager.RegistMouseEvent(EKeyType::RButton, EButtonState::Released, std::make_unique<CSetMouseEventToMouseManager>(MouseManager));

	World.AddNewObjectTypeEvent(CMousePointer::GetStaticType(), std::make_unique<CBindMousePointer>(Window, MouseManager));
}