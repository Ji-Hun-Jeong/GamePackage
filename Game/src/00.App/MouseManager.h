#pragma once
#include <Core/public/InputManager.h>

class CMouseManager
{
	SINGLE(CMouseManager)
public:
	void SetStateByInputManager(const Core::CInputManager& InInputManager)
	{
		bLButtonClicked = InInputManager.CorrectKeyState(EKeyType::LButton, EButtonState::Tap);
		bLButtonHold = InInputManager.CorrectKeyState(EKeyType::LButton, EButtonState::Hold);
		bLButtonReleased = InInputManager.CorrectKeyState(EKeyType::LButton, EButtonState::Released);

		bRButtonClicked = InInputManager.CorrectKeyState(EKeyType::RButton, EButtonState::Tap);
		bRButtonHold = InInputManager.CorrectKeyState(EKeyType::RButton, EButtonState::Hold);
		bRButtonReleased = InInputManager.CorrectKeyState(EKeyType::RButton, EButtonState::Released);
	}
	void SetMousePosition(const Vector2& InMousePosition) { MousePosition = InMousePosition; }
	const Vector2& GetMousePosition() const { return MousePosition; }

	bool IsLButtonClicked() const { return bLButtonClicked; }
	bool IsLButtonHold() const { return bLButtonHold; }
	bool IsLButtonReleased() const { return bLButtonReleased; }

	bool IsRButtonClicked() const { return bRButtonClicked; }
	bool IsRButtonHold() const { return bRButtonHold; }
	bool IsRButtonReleased() const { return bRButtonReleased; }

private:
	Vector2 MousePosition;

	bool bLButtonClicked = false;
	bool bLButtonHold = false;
	bool bLButtonReleased = false;

	bool bRButtonClicked = false;
	bool bRButtonHold = false;
	bool bRButtonReleased = false;

};

#define LClicked() CMouseManager::GetInst().IsLButtonClicked()
#define LHold() CMouseManager::GetInst().IsLButtonHold()
#define LReleased() CMouseManager::GetInst().IsLButtonReleased()

#define RClicked() CMouseManager::GetInst().IsRButtonClicked()
#define RHold() CMouseManager::GetInst().IsRButtonHold()
#define RReleased() CMouseManager::GetInst().IsRButtonReleased()