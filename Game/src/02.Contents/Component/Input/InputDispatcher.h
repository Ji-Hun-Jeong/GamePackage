#pragma once
#include "01.Base/Actor/Actor.h"
#include "InputComponent.h"

class CInputExecuter
{
private:
	class CActionInfo
	{
		friend class CInputExecuter;
	public:
		CActionInfo(uint32_t InInputLayer, const std::function<void()>& InAction)
			: InputLayer(InInputLayer), Action(InAction)
		{
		}
		~CActionInfo() = default;
	private:
		uint32_t InputLayer;
		const std::function<void()>& Action;
	};
public:
	CInputExecuter() = default;
	~CInputExecuter() = default;
public:
	void ExecuteAction()
	{
		for (auto& ReadyAction : ReadyActions)
			if (ReadyAction.InputLayer == MinLayer)
				if (ReadyAction.Action)
					ReadyAction.Action();

		ReadyActions.clear();
		MinLayer = -1;
	}
	void AddAction(uint32_t InInputLayer, const std::function<void()>& InAction)
	{
		for (auto& ActionInfo : ReadyActions)
		{
			if (&ActionInfo.Action == &InAction)
				return;
		}

		ReadyActions.emplace_back(InInputLayer, InAction);

		MinLayer = std::min<uint32_t>(MinLayer, InInputLayer);
	}

private:
	std::vector<CActionInfo> ReadyActions;
	uint32_t MinLayer = -1;

};

class CInputDispatcher : public CActor
{
	GENERATE_OBJECT(CInputDispatcher)
public:
	CInputDispatcher() = default;
	~CInputDispatcher() = default;

public:
	void Update(float InDeltaTime) override
	{
		for (auto Iter = InputComponents.begin(); Iter != InputComponents.end();)
		{
			if (Iter->IsValid() == false)
			{
				Iter = InputComponents.erase(Iter);
				continue;
			}
				
			CInputComponent* InputComponent = Iter->Get();
			MakeActionList(*InputComponent);
			++Iter;
		}

		for (auto& Pair : InputExecuters)
		{
			Pair.second.ExecuteAction();
		}
	}
	void RegistInput(CInputComponent& InInputComponent)
	{
		for (auto InputComponent : InputComponents)
			if (InputComponent == &InInputComponent)
				return;

		InputComponents.emplace_back(&InInputComponent);
	}

private:
	void MakeActionList(const CInputComponent& InInputComponent)
	{
		uint32_t InputLayer = InInputComponent.GetInputLayer();
		auto& Actions = InInputComponent.GetActions();
		for (auto& Pair : Actions)
		{
			uint32_t Key = Pair.first;
			const std::function<void()>& Action = Pair.second;

			UInputKey InputKey = CInputComponent::Decode(Key);

			if (GetKey(InputKey.Key.KeyType, InputKey.Key.ButtonState) == false)
				continue;

			if (InputExecuters.contains(Key) == false)
				InputExecuters.emplace(Key, CInputExecuter{});

			auto Iter = InputExecuters.find(Key);
			Iter->second.AddAction(InputLayer, Action);
		}
	}

private:
	std::vector<CObjectPtr<CInputComponent>> InputComponents;

	std::map<uint32_t, CInputExecuter> InputExecuters;

};

