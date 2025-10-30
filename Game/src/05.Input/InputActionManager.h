#pragma once
#include <Core/public/InputManager.h>

struct TKeyState
{
	EKeyType KeyType;
	EButtonState ButtonState;
};

class CInputActionValue
{
	friend class CInputActionManager;
public:
	CInputActionValue(std::function<void()> InAction)
		: Action(InAction)
		, bUse(true)
	{}
	~CInputActionValue() = default;

public:
	void AddKeyCondition(const TKeyState& InKeyState)
	{
		KeyStates.push_back(InKeyState);
	}
	void PerformAction(Core::CInputManager& InInputManager) const
	{
		if (bUse == false)
			return;

		bool bAllSatisfy = true;
		for (const TKeyState& KeyState : KeyStates)
		{
			bAllSatisfy = InInputManager.CorrectKeyState(KeyState.KeyType, KeyState.ButtonState);
			if (bAllSatisfy == false)
				return;
		}
		if (bAllSatisfy)
			Action();
	}

private:
	std::vector<TKeyState> KeyStates;
	std::function<void()> Action;

	bool bUse;

};

class CInputActionValueCollector
{
public:
	CInputActionValueCollector() = default;
	~CInputActionValueCollector() = default;

public:
	void PushInputActionValue(const CInputActionValue& InInputActionValue)
	{
		InputActionValues.push_back(&InInputActionValue);
	}
	const std::vector<const CInputActionValue*>& GetInputActionValues() const { return InputActionValues; }
private:
	std::vector<const CInputActionValue*> InputActionValues;

};

class CInputActionManager
{
public:
	CInputActionManager(Core::CInputManager& InInputManager)
		: InputManager(InInputManager)
	{}
	~CInputActionManager() = default;

public:
	void PerformAction(const CInputActionValueCollector& InInputActionValueCollector) const
	{
		for (const CInputActionValue* InputActionValue : InInputActionValueCollector.GetInputActionValues())
		{
			InputActionValue->PerformAction(InputManager);
		}
	}

private:
	Core::CInputManager& InputManager;

};

