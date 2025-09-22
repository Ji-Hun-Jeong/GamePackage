#pragma once

namespace Core
{
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
		LButton, RButton, MButton,
		_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		End,
	};

	union UInputBlendValue
	{
		struct
		{
			EKeyType KeyType;
			EButtonState ButtonState;
		}BlendKey;
		UINT RealKey;
	};

	class IInputEvent
	{
		friend class CInputManager;
		virtual void OnActivate(EKeyType InKeyType, EButtonState InButtonState) = 0;
	};

	class CInputManager
	{
	public:
		CInputManager() = default;
		~CInputManager() = default;

	public:
		void Update();

		void RegistInputEvent(EKeyType InKeyType, EButtonState InButtonState, std::unique_ptr<IInputEvent> InputEvent)
		{
			UInputBlendValue InputBlendValue;
			InputBlendValue.BlendKey.KeyType = InKeyType;
			InputBlendValue.BlendKey.ButtonState = InButtonState;

			UINT Key = InputBlendValue.RealKey;
			InputEvents[Key].push_back(std::move(InputEvent));
		}

	private:
		EButtonState KeyStates[(UINT)EKeyType::End];
		
		std::unordered_map<UINT, std::vector<std::unique_ptr<IInputEvent>>> InputEvents;

	};
}