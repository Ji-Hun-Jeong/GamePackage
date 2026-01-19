#pragma once
#include <Core/public/InputManager.h>
#include "01.Base/Actor/Component/Component.h"
struct TInputState
{
	EKeyType KeyType;
	EButtonState ButtonState;
	std::function<void()> Action;
};

union UInputKey
{
	struct
	{
		EKeyType KeyType;
		EButtonState ButtonState;
	} Key;
	uint32_t Value;
};

class CInputComponent : public CComponent
{
	GENERATE_OBJECT(CInputComponent)
public:
	CInputComponent() = default;
	~CInputComponent() = default;

public:
	void AddAction(EKeyType InKeyType, EButtonState InButtonState, const std::function<void()>& InAction)
	{
		uint32_t InCodeValue = InCode(InKeyType, InButtonState);
		
		Actions.emplace(InCodeValue, InAction);
	}
	
	void SetInputLayer(uint32_t InInputLayer) { InputLayer = InInputLayer; }
	uint32_t GetInputLayer() const { return InputLayer; }

	const std::map<uint32_t, std::function<void()>>& GetActions() const { return Actions; }

public:
	static uint32_t InCode(EKeyType InKeyType, EButtonState InButtonState)
	{
		UInputKey InputKey;
		InputKey.Key.KeyType = InKeyType;
		InputKey.Key.ButtonState = InButtonState;

		return InputKey.Value;
	}
	static UInputKey Decode(uint32_t InValue)
	{
		UInputKey InputKey;
		InputKey.Value = InValue;
		return InputKey;
	}

private:
	std::map<uint32_t, std::function<void()>> Actions;

	uint32_t InputLayer = 0;

};