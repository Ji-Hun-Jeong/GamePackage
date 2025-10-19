#pragma once
#include <Core/public/InputManager.h>

class CInputActionValue
{
	friend class CInputActionManager;
	CInputActionValue(EKeyType InKeyType, EButtonState InButtonState, Core::IKeyEvent* InBoundKeyEvent
		, std::function<void()> InInputAction)
		: InputAction(InInputAction)
		, bDestroy(false)
		, KeyType(InKeyType)
		, ButtonState(InButtonState)
		, BoundKeyEvent(InBoundKeyEvent)
		, bSatisfy(false)
	{
		assert(InInputAction);
		assert(BoundKeyEvent);
	}
public:
	~CInputActionValue() = default;

public:
	bool IsSatisfy() const { return bSatisfy; }
	void SetSatisfy(bool bInSatisfy) { bSatisfy = bInSatisfy; }
	void Destroy() { bDestroy = true; }

	const std::function<void()>& GetInputAction() const { return InputAction; }
	void SetInputAction(std::function<void()> InInputAction) { InputAction = InInputAction; }

private:
	std::function<void()> InputAction;

	EKeyType KeyType;
	EButtonState ButtonState;
	Core::IKeyEvent* BoundKeyEvent;

	bool bSatisfy;
	bool bDestroy;

};

class CInputActionValueSetSatisfy : public Core::IKeyEvent
{
public:
	CInputActionValueSetSatisfy()
		: InputActionValue(nullptr)
	{}
	void SetRefInputActionValue(CInputActionValue* InInputActionValue) { InputActionValue = InInputActionValue; }
	void OnActivate(EKeyType InKeyType, EButtonState InButtonState) override
	{
		InputActionValue->SetSatisfy(true);
	}
private:
	CInputActionValue* InputActionValue;
};

class CInputActionManager
{
public:
	CInputActionManager(Core::CInputManager& InInputManager)
		: InputManager(InInputManager)
	{}
	~CInputActionManager() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	void PerformAction()
	{
		InputManager.Update();

		for (size_t i = 0; i < InputActionValues.size();)
		{
			auto& InputActionValue = InputActionValues[i];
			if (InputActionValue->bDestroy)
			{
				InputManager.DeRegistKeyEvent(InputActionValue->KeyType, InputActionValue->ButtonState, InputActionValue->BoundKeyEvent);
				InputActionValues[i] = std::move(InputActionValues.back());
				InputActionValues.pop_back();
			}
			else
			{
				i += 1;

				if (InputActionValue->IsSatisfy() == false)
					continue;

				const std::function<void()>& InputAction = InputActionValue->GetInputAction();
				if (InputAction)
					InputAction();
				InputActionValue->SetSatisfy(false);
			}
		}
	}
	CInputActionValue* AddInputActionValue(EKeyType InKeyType, EButtonState InButtonState, std::function<void()> InInputAction)
	{
		CInputActionValueSetSatisfy* SetSatisfyEvent = new CInputActionValueSetSatisfy;
		CInputActionValue* InputActionValue = new CInputActionValue(InKeyType, InButtonState, SetSatisfyEvent, InInputAction);
		SetSatisfyEvent->SetRefInputActionValue(InputActionValue);

		InputManager.RegistKeyEvent(InKeyType, InButtonState
			, std::unique_ptr<CInputActionValueSetSatisfy>(SetSatisfyEvent));

		InputActionValues.emplace_back(InputActionValue);
		return InputActionValue;
	}

private:
	Core::CInputManager& InputManager;

	std::vector<std::unique_ptr<CInputActionValue>> InputActionValues;
};

