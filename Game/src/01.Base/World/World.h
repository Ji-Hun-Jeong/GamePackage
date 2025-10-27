#pragma once
#include "WorldEvent.h"
#include "01.Base/Actor/Actor.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

public:
	void Start();

	void Arrange();

	void Ready();

	void Update();

	void CaptureSnapShot();

	template <typename T>
	T* SpawnActor(CActor* InOwnerActor = nullptr)
	{
		T* Actor = NewObject<T>(InOwnerActor);
		Actor->World = this;

		if (InOwnerActor)
			Actor->GetOwner()->Attach(InOwnerActor);

		auto Iter = NewObjectTypeEvents.find(Actor->GetType());
		if (Iter != NewObjectTypeEvents.end())
		{
			for (auto& NewObjectTypeEvent : Iter->second)
				NewObjectTypeEvent->CreatedInWorld(*this, *Actor);
		}
		for (auto& NewObjectEvent : NewObjectEvents)
			NewObjectEvent->CreatedInWorld(*this, *Actor);

		NextAddedWorldActors.emplace(Actor);

		return Actor;
	}
	
public:
	template <typename T_SCENE>
	void LoadScene()
	{
		PushWorldSynchronizeEvent([this]()->void
			{
				while (NextAddedWorldActors.empty() == false)
					NextAddedWorldActors.pop();

				for (auto& WorldActor : WorldActors)
					DeleteObject(WorldActor);
				SpawnActor<T_SCENE>();
			});
	}

	void Serialize(const class CActor& InSerializeActor, const std::string& InSavePath);

	CObject* Deserialize(const std::string& InReadPath, CActor* InOwnerActor);

	void PushWorldSynchronizeEvent(std::function<void()> InWorldSynchronizeEvent) { WorldSynchronizeEvents.push(InWorldSynchronizeEvent); }

public:
	const std::vector<CActor*>& GetWorldActors() const { return WorldActors; }
	void AddNewObjectTypeEvent(ObjectType InObjectType, std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		auto Iter = NewObjectTypeEvents.find(InObjectType);
		if (Iter == NewObjectTypeEvents.end())
		{
			std::vector<std::unique_ptr<INewObjectEvent>> Vec;
			Vec.push_back(std::move(InNewObjectEvent));
			NewObjectTypeEvents.insert({ InObjectType, std::move(Vec) });
		}
		else
			Iter->second.push_back(std::move(InNewObjectEvent));
	}
	void AddNewObjectEvent(std::unique_ptr<INewObjectEvent> InNewObjectEvent)
	{
		NewObjectEvents.push_back(std::move(InNewObjectEvent));
	}

private:
	std::vector<CActor*> WorldActors;
	std::queue<CActor*> NextAddedWorldActors;

	std::map<ObjectType, std::vector<std::unique_ptr<INewObjectEvent>>> NewObjectTypeEvents;
	std::vector<std::unique_ptr<INewObjectEvent>> NewObjectEvents;

	std::queue<std::function<void()>> WorldSynchronizeEvents;

};