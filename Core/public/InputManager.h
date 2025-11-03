#pragma once
#include <queue>

enum class EButtonState : uint16_t
{
	None,
	Tap,
	Hold,
	Released,
};
enum class EKeyType : uint16_t
{
	Tab, Shift, Ctrl, Alt, Esc, Space, Left, Up, Right, Down,
	_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	LButton, RButton, MButton,
	End,
};

namespace Core
{
	class CInputManager
	{
		friend class CSetMousePosition;
	public:
		CInputManager() = default;
		~CInputManager() = default;

	public:
		void Update();
		bool CorrectKeyState(EKeyType InKeyType, EButtonState InButtonState) const { return KeyStates[(UINT)InKeyType] == InButtonState; }

	private:
		EButtonState KeyStates[(UINT)EKeyType::End];
	
	};
}