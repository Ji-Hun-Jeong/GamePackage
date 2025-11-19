#include "pch.h"
#include "InputManager.h"

namespace Core
{
	CInputManager::CInputManager()
		: KeyStates{}
	{

	}
	const char KeyMapping[] =
	{
		VK_TAB, VK_SHIFT, VK_CONTROL, VK_MENU, VK_ESCAPE, VK_SPACE, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN,
		'0','1','2','3','4','5','6','7','8','9',
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
		VK_LBUTTON, VK_RBUTTON, VK_MBUTTON
	};
	bool bPressed[(UINT)EKeyType::End] = { 0 };

	void CInputManager::Update()
	{
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
	}
}