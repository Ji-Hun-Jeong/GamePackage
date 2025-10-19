#include "pch.h"
#include "Game.h"

class CSetMousePositionToMouseManager : public Core::IMouseEvent
{
public:
	CSetMousePositionToMouseManager(CMouseManager& InMouseManager)
		: MouseManager(InMouseManager)
	{}
	
private:
	void OnActivate(EKeyType InKeyType, EButtonState InButtonState, const TMouseScreenPosition& InMousePosition) override
	{
		if (InButtonState == EButtonState::Tap)
			MouseManager.SetMouseClick(true);
		if (InButtonState == EButtonState::Released)
			MouseManager.SetMouseRelease(true);
	}
	CMouseManager& MouseManager;
};

void CGame::Binding()
{
	InputManager.RegistMouseEvent(EKeyType::LButton, EButtonState::Tap, std::make_unique<CSetMousePositionToMouseManager>(MouseManager));
	InputManager.RegistMouseEvent(EKeyType::LButton, EButtonState::Released, std::make_unique<CSetMousePositionToMouseManager>(MouseManager));
}