#pragma once
#include "WorldEvent.h"
#include "01.Base/Object/Actor/Actor.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

public:
	void Update()
	{
		while (NextAddedWorldActors.empty() == false)
		{
			std::unique_ptr<CActor> Actor = std::move(NextAddedWorldActors.front());
			NextAddedWorldActors.pop();

			Actor->BeginPlay();

			for (auto& ObjectEnterWorldEvent : ObjectEnterWorldEvents)
				ObjectEnterWorldEvent->EnterWorld(*this, *Actor);

			WorldActors.push_back(std::move(Actor));
		}

		for (auto& WorldActor : WorldActors)
			WorldActor->Update(0.5f);

		if (bFlagDestroyedWorldObject == false)
			return;

		auto removeIter = std::remove_if(WorldActors.begin(), WorldActors.end(),
			[&](const std::unique_ptr<CActor>& InActor) {
				if (InActor->bDestroy)
				{
					NumberGenerator.ReleaseNumber(InActor->GetInstanceId());
					for (auto& ObjectExitWorldEvent : ObjectExitWorldEvents)
						ObjectExitWorldEvent->ExitWorld(*this, *InActor.get());
				}
				return InActor->bDestroy;
			});

		WorldActors.erase(removeIter, WorldActors.end());

		bFlagDestroyedWorldObject = false;
	}

	template <typename T>
	std::unique_ptr<T> NewObject()
	{
		T* Object = new T;
		Object->InstanceId = NumberGenerator.GenerateNumber();
		Object->World = this;

		for (auto& NewObjectEvent : NewObjectEvents)
			NewObjectEvent->CreatedInWorld(*this, *Object);

		return std::unique_ptr<T>(Object);
	}

	template <typename T>
	T* SpawnActor(CActor* InOwner = nullptr)
	{
		std::unique_ptr<T> Actor = NewObject<T>();

		if (InOwner)
			InOwner->AttachChild(Actor.get());

		NextAddedWorldActors.push(std::move(Actor));

		return Actor.get();
	}

	const std::vector<std::unique_ptr<CActor>>& GetWorldActors() const { return WorldActors; }
	void MarkDestroyed() { bFlagDestroyedWorldObject = true; }
	void AddObjectEnterWorldEvent(std::unique_ptr<IObjectEnterWorld> InObjectEnterWorldEvent)
	{
		ObjectEnterWorldEvents.push_back(std::move(InObjectEnterWorldEvent));
	}
	void AddObjectExitWorldEvent(std::unique_ptr<IObjectExitWorld> InObjectExitWorldEvent)
	{
		ObjectExitWorldEvents.push_back(std::move(InObjectExitWorldEvent));
	}
	void AddNewObjectEvent(std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		NewObjectEvents.push_back(std::move(InNewObjectEvent));
	}

private:
	std::vector<std::unique_ptr<CActor>> WorldActors;
	std::queue<std::unique_ptr<CActor>> NextAddedWorldActors;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;

	std::vector<std::unique_ptr<IObjectEnterWorld>> ObjectEnterWorldEvents;
	std::vector<std::unique_ptr<IObjectExitWorld>> ObjectExitWorldEvents;
	std::vector<std::unique_ptr<INewObjectEvent>> NewObjectEvents;
};