#pragma once
#include "WorldEvent.h"
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/Component/Collider/CollisionManager.h"

class CWorld : public CObject
{
	GENERATE_OBJECT(CWorld)
public:
	CWorld();
	~CWorld();

public:
	void Start();

	void Arrange()
	{
		for (auto Iter = WorldActors.begin(); Iter != WorldActors.end();)
		{
			CActor* Actor = *Iter;
			if (Actor->IsDestroy())
			{
				Actor->EndPlay();
				if (Actor->GetOwner())
					Actor->GetOwner()->Detach(Actor);
				Iter = WorldActors.erase(Iter);
			}
			else
				++Iter;
		}
	}

	void Ready()
	{
		while (NextAddedWorldActors.empty() == false)
		{
			CActor* Actor = NextAddedWorldActors.front();
			NextAddedWorldActors.pop();

			Actor->BeginPlay();

			WorldActors.push_back(Actor);
		}

		while (WorldSynchronizeEvents.empty() == false)
		{
			WorldSynchronizeEvents.front()();
			WorldSynchronizeEvents.pop();
		}
	}

	void Update()
	{
		for (auto& WorldActor : WorldActors)
		{
			if (WorldActor->IsActivate() == false)
				continue;
			WorldActor->Update(1.0f / 60.0f);

			std::vector<CCollider*> Colliders = WorldActor->GetComponents<CCollider>();
			for (auto Collider : Colliders)
				CollisionManager.RequestCollision(*Collider);
		}

		CollisionManager.CheckCollisionProcess();
	}

	void CaptureSnapShot()
	{
		for (auto& WorldActor : WorldActors)
		{
			if (WorldActor->IsActivate() == false)
				continue;
			WorldActor->CaptureSnapShot();
		}
	}

	template <typename T>
	T* SpawnActor(CActor* InOwnerActor = nullptr, CClass* InClass = nullptr)
	{
		T* Actor = nullptr;
		Actor = NewObject<T>(InOwnerActor, InClass);

		if (InOwnerActor)
			InOwnerActor->Attach(Actor);

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

	template <typename T_SCENE>
	void LoadScene()
	{
		while (NextAddedWorldActors.empty() == false)
		{
			DestroyObject(NextAddedWorldActors.front());
			NextAddedWorldActors.pop();
		}

		for (auto& WorldActor : WorldActors)
			DestroyObject(WorldActor);
		// Todo: 나중에 상태패턴 또는 메세지큐로 전환
		PushWorldSynchronizeEvent([this]()->void
			{ 
				SpawnActor<T_SCENE>();
			});
	}
	void LoadSceneByClass(CClass* InSceneClass)
	{
		while (NextAddedWorldActors.empty() == false)
		{
			DestroyObject(NextAddedWorldActors.front());
			NextAddedWorldActors.pop();
		}

		for (auto& WorldActor : WorldActors)
			DestroyObject(WorldActor);

		PushWorldSynchronizeEvent([this, InSceneClass]()->void
			{
				SpawnActor<CActor>(nullptr, InSceneClass);
			});
	}

	void Serialize(const class CActor& InSerializeActor, const std::string& InSavePath);

	CObject* Deserialize(const std::string& InReadPath, CActor* InOwnerActor);

	void PushWorldSynchronizeEvent(std::function<void()> InWorldSynchronizeEvent) { WorldSynchronizeEvents.push(InWorldSynchronizeEvent); }

	void RenderWorld(class CSpriteRenderer& InRenderer);

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

	CCollisionManager CollisionManager;

};