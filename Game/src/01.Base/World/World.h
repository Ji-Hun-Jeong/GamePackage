#pragma once
#include <nlohmann/json.hpp>
#include "WorldEvent.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"
using CSerializer = nlohmann::json;
using ObjectType = size_t;

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

	void Serialize(const class CActor& InSerializeActor, const std::string& InSavePath);

	void Deserialize(class CActor& InSerializeActor, const std::string& InReadPath);

	void PushWorldSynchronizeEvent(std::function<void()> InWorldSynchronizeEvent) { WorldSynchronizeEvents.push(InWorldSynchronizeEvent); }

	void InitalizeObject(class CObject* InObject, class CActor* InOwnerActor);
	void AppearActor(class CActor* InActor);
	void ClearWorld();
	void ResetObject(class CObject* InObject, class CActor* InOwnerActor);
	void DisappearActor(class CActor* InActor, class CActor* InOwnerActor);
	
public:
	const std::vector<std::unique_ptr<CActor>>& GetWorldActors() const { return WorldActors; }
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
	std::vector<std::unique_ptr<class CActor>> WorldActors;
	std::queue<std::unique_ptr<class CActor>> NextAddedWorldActors;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;
	
	std::map<ObjectType, std::vector<std::unique_ptr<INewObjectEvent>>> NewObjectTypeEvents;
	std::vector<std::unique_ptr<INewObjectEvent>> NewObjectEvents;

	std::queue<std::function<void()>> WorldSynchronizeEvents;

};

extern CWorld* g_World;

template <typename T>
T* NewObject(class CActor* InOwnerActor)
{
	T* Object = new T;
	g_World->InitalizeObject(Object, InOwnerActor);
	return Object;
}

template <typename T>
T* SpawnActor(class CActor* InOwnerActor)
{
	T* Actor = NewObject<T>(InOwnerActor);
	g_World->AppearActor(Actor);
	return Actor;
}

template <typename T_SCENE>
void LoadScene()
{
	g_World->PushWorldSynchronizeEvent([]()->void
		{
			g_World->ClearWorld();
			SpawnActor<T_SCENE>();
		});
}