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

			WorldActors.push_back(std::move(Actor));
		}

		for (auto& WorldActor : WorldActors)
			WorldActor->Update(0.5f);

		if (bFlagDestroyedWorldObject == false)
			return;

		auto RemoveStartIter = std::remove_if(WorldActors.begin(), WorldActors.end(),
			[](const std::unique_ptr<CActor>& InActor)->bool
			{
				return InActor->bDestroy;
			});

		WorldActors.erase(RemoveStartIter, WorldActors.end());

		bFlagDestroyedWorldObject = false;
	}

	template <typename T>
	std::unique_ptr<T> NewObject()
	{
		T* Object = new T;
		Object->InstanceId = NumberGenerator.GenerateNumber();
		Object->World = this;

		auto Iter = NewObjectEvents.find(T::GetStaticType());
		if (Iter != NewObjectEvents.end())
		{
			for (auto& NewObjectEvent : Iter->second)
				NewObjectEvent->CreatedInWorld(*this, *Object);
		}

		return std::unique_ptr<T>(Object);
	}

	template <typename T>
	T* SpawnActor(CActor* InOwner = nullptr)
	{
		std::unique_ptr<T> Actor = NewObject<T>();
		T* RawActor = Actor.get();

		if (InOwner)
			InOwner->AttachChild(Actor.get());

		NextAddedWorldActors.push(std::move(Actor));

		return RawActor;
	}

	const std::vector<std::unique_ptr<CActor>>& GetWorldActors() const { return WorldActors; }
	void MarkDestroyed() { bFlagDestroyedWorldObject = true; }
	void AddNewObjectEvent(ObjectType InObjectType, std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		auto Iter = NewObjectEvents.find(InObjectType);
		if (Iter == NewObjectEvents.end())
		{
			std::vector<std::unique_ptr<INewObjectEvent>> Vec;
			Vec.push_back(std::move(InNewObjectEvent));
			NewObjectEvents.insert({ InObjectType, std::move(Vec) });
		}
		else
			Iter->second.push_back(std::move(InNewObjectEvent));
	}

private:
	std::vector<std::unique_ptr<CActor>> WorldActors;
	std::queue<std::unique_ptr<CActor>> NextAddedWorldActors;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;
	
	std::map<ObjectType, std::vector<std::unique_ptr<INewObjectEvent>>> NewObjectEvents;
};