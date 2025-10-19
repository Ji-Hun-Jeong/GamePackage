#pragma once
#include "WorldEvent.h"
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Scene.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

public:
	void Start();

	void Arrange()
	{
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

	void Ready()
	{
		while (NextAddedWorldActors.empty() == false)
		{
			std::unique_ptr<CActor> Actor = std::move(NextAddedWorldActors.front());
			NextAddedWorldActors.pop();

			Actor->BeginPlay();

			WorldActors.push_back(std::move(Actor));
		}
		while (WorldFirstCommands.empty() == false)
		{
			WorldFirstCommands.front()();
			WorldFirstCommands.pop();
		}
	}

	void Update()
	{
		for (auto& WorldActor : WorldActors)
			WorldActor->Update(1.0f / 60.0f);

		for (auto& WorldActor : WorldActors)
			WorldActor->FinalUpdate();
	}

	void CaptureSnapShot()
	{
		for (auto& WorldActor : WorldActors)
			WorldActor->CaptureSnapShot();
	}

	void Seralize(const std::string& InSavePath)
	{
		CSerializer ActorArrayData = CSerializer::array();
		CSerializer ActorData;
		for (auto& WorldActor : WorldActors)
		{
			WorldActor->Serialize(ActorData);
			ActorArrayData.push_back(ActorData);
		}
		Serializer["objects"] = ActorArrayData;
		std::cout << Serializer.dump(4);
	}

	template <typename T>
	std::unique_ptr<T> NewObject()
	{
		T* Object = new T;
		Object->InstanceId = NumberGenerator.GenerateNumber();
		Object->World = this;

		auto Iter = NewObjectTypeEvents.find(T::GetStaticType());
		if (Iter != NewObjectTypeEvents.end())
		{
			for (auto& NewObjectTypeEvent : Iter->second)
				NewObjectTypeEvent->CreatedInWorld(*this, *Object);
		}
		for (auto& NewObjectEvent : NewObjectEvents)
			NewObjectEvent->CreatedInWorld(*this, *Object);

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

		RawActor->Initalize();

		return RawActor;
	}

	template <typename T_SCENE>
	void LoadScene()
	{
		WorldFirstCommands.push([this]()->void
			{
				LoadSceneImmediate<T_SCENE>();
			});
	}
private:
	template <typename T_SCENE>
	void LoadSceneImmediate()
	{
		while (NextAddedWorldActors.empty() == false)
			NextAddedWorldActors.pop();

		for (auto& WorldActor : WorldActors)
			WorldActor->Destroy();
		SpawnActor<T_SCENE>();
	}
public:
	const std::vector<std::unique_ptr<CActor>>& GetWorldActors() const { return WorldActors; }
	void MarkDestroyed() { bFlagDestroyedWorldObject = true; }
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
	std::vector<std::unique_ptr<CActor>> WorldActors;
	std::queue<std::unique_ptr<CActor>> NextAddedWorldActors;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;
	
	std::map<ObjectType, std::vector<std::unique_ptr<INewObjectEvent>>> NewObjectTypeEvents;
	std::vector<std::unique_ptr<INewObjectEvent>> NewObjectEvents;

	CSerializer Serializer;

	std::queue<std::function<void()>> WorldFirstCommands;
};