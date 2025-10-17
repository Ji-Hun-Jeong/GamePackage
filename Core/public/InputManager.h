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

struct TMousePosition
{
	int MouseX;
	int MouseY;
};

namespace Core
{
	union UInputBlendValue
	{
		struct
		{
			EKeyType KeyType;
			EButtonState ButtonState;
		}BlendKey;
		UINT RealKey;
	};

	class IKeyEvent
	{
		friend class CInputManager;
		virtual void OnActivate(EKeyType InKeyType, EButtonState InButtonState) = 0;
	};

	class IMouseEvent
	{
		friend class CInputManager;
		virtual void OnActivate(EKeyType InKeyType, EButtonState InButtonState, const TMousePosition& InMousePosition) = 0;
	};

	class CInputManager
	{
		friend class CSetMousePosition;
	public:
		CInputManager(class CWindow& InWindow);
		~CInputManager() = default;

	public:
		void Update();

		void RegistKeyEvent(EKeyType InKeyType, EButtonState InButtonState, std::unique_ptr<IKeyEvent> InKeyEvent)
		{
			UInputBlendValue InputBlendValue;
			InputBlendValue.BlendKey.KeyType = InKeyType;
			InputBlendValue.BlendKey.ButtonState = InButtonState;

			UINT Key = InputBlendValue.RealKey;
			KeyEvents[Key].push_back(std::move(InKeyEvent));
		}

		void RegistMouseEvent(EKeyType InKeyType, EButtonState InButtonState, std::unique_ptr<IMouseEvent> InMouseEvent)
		{
			UInputBlendValue InputBlendValue;
			InputBlendValue.BlendKey.KeyType = InKeyType;
			InputBlendValue.BlendKey.ButtonState = InButtonState;

			UINT Key = InputBlendValue.RealKey;
			MouseEvents[Key].push_back(std::move(InMouseEvent));
		}

		void DeRegistKeyEvent(EKeyType InKeyType, EButtonState InButtonState, IKeyEvent* InKeyEvent)
		{
			UInputBlendValue InputBlendValue;
			InputBlendValue.BlendKey.KeyType = InKeyType;
			InputBlendValue.BlendKey.ButtonState = InButtonState;

			UINT Key = InputBlendValue.RealKey;
			DeRegistEvents.emplace(Key, InKeyEvent);
		}

		const TMousePosition& GetMousePosition() const { return MousePosition; }

	private:
		bool CorrectKeyState(EKeyType InKeyType, EButtonState InButtonState) { return KeyStates[(UINT)InKeyType] == InButtonState; }

	private:
		EButtonState KeyStates[(UINT)EKeyType::End];
		
		std::unordered_map<UINT, std::vector<std::unique_ptr<IKeyEvent>>> KeyEvents;
		std::unordered_map<UINT, std::vector<std::unique_ptr<IMouseEvent>>> MouseEvents;

		std::queue<std::pair<UINT, IKeyEvent*>> DeRegistEvents;

		TMousePosition MousePosition;

	};
}