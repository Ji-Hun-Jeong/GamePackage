#include "pch.h"
#include "InputManager.h"
#include "Window.h"

namespace Core
{
	const char KeyMapping[] =
	{
		VK_TAB, VK_SHIFT, VK_CONTROL, VK_MENU, VK_ESCAPE, VK_SPACE, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		VK_LBUTTON, VK_RBUTTON, VK_MBUTTON
	};
	bool bPressed[(UINT)EKeyType::End] = { 0 };

	class CSetMousePosition : public IMouseMove
	{
	public:
		CSetMousePosition(CInputManager& InInputManager)
			: InputManager(InInputManager)
		{}
	public:
		void MouseMove(int InX, int InY, uint32_t InScreenWidth, uint32_t InScreenHeight) override
		{
			InputManager.MouseScreenPosition.MouseX = InX;
			InputManager.MouseScreenPosition.MouseY = InY;
		}

	private:
		CInputManager& InputManager;

	};

	CInputManager::CInputManager(CWindow& InWindow)
		: MouseScreenPosition{}
	{
		InWindow.RegistMouseMoveEvent(std::make_unique<CSetMousePosition>(*this));
	}

	void CInputManager::Update()
	{
		while (DeRegistEvents.empty() == false)
		{
			auto Pair = DeRegistEvents.front();
			DeRegistEvents.pop();

			auto& RefKeyEvents = KeyEvents[Pair.first];
			for (size_t i = 0; i < RefKeyEvents.size(); ++i)
			{
				if (RefKeyEvents[i].get() == Pair.second)
				{
					RefKeyEvents[i] = std::move(RefKeyEvents.back());
					RefKeyEvents.pop_back();
					break;
				}
			}
		}
		for (UINT i = 0; i < (UINT)EKeyType::End; ++i)
		{
			if (GetAsyncKeyState(KeyMapping[i]) & 0x8000)
			{
				if (bPressed[i])
					KeyStates[i] = EButtonState::Hold;
				else
					KeyStates[i] = EButtonState::Tap;
				bPressed[i] = true;
			}
			else
			{
				if (bPressed[i])
					KeyStates[i] = EButtonState::Released;
				else
					KeyStates[i] = EButtonState::None;
				bPressed[i] = false;
			}
		}

		UInputBlendValue InputBlendValue;
		for (auto& Pair : KeyEvents)
		{
			InputBlendValue.RealKey = Pair.first;
			if (CorrectKeyState(InputBlendValue.BlendKey.KeyType, InputBlendValue.BlendKey.ButtonState) == false)
				continue;

			for (auto& KeyEvent : Pair.second)
				KeyEvent->OnActivate(InputBlendValue.BlendKey.KeyType, InputBlendValue.BlendKey.ButtonState);
		}

		for (auto& Pair : MouseEvents)
		{
			InputBlendValue.RealKey = Pair.first;
			if (CorrectKeyState(InputBlendValue.BlendKey.KeyType, InputBlendValue.BlendKey.ButtonState) == false)
				continue;

			for (auto& MouseEvent : Pair.second)
				MouseEvent->OnActivate(InputBlendValue.BlendKey.KeyType, InputBlendValue.BlendKey.ButtonState, MouseScreenPosition);
		}
	}
}