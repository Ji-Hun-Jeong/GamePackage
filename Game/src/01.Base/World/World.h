#pragma once
#include "WorldEvent.h"
#include "01.Base/Object/Object.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

public:
	void Update()
	{
		while (NextAddedWorldObjects.empty() == false)
		{
			CObject* Object = NextAddedWorldObjects.front();
			NextAddedWorldObjects.pop();

			Object->BeginPlay();

			for (auto& ObjectEnterWorldEvent : ObjectEnterWorldEvents)
				ObjectEnterWorldEvent->EnterWorld(*this, *Object);

			WorldObjects.emplace_back(Object);
		}

		for (auto& WorldObject : WorldObjects)
			WorldObject->Update(0.5f);

		if (bFlagDestroyedWorldObject == false)
			return;

		auto removeIter = std::remove_if(WorldObjects.begin(), WorldObjects.end(),
			[&](const std::unique_ptr<CObject>& Object) {
				if (Object->bDestroy)
				{
					NumberGenerator.ReleaseNumber(Object->GetInstanceId());
					for (auto& ObjectExitWorldEvent : ObjectExitWorldEvents)
						ObjectExitWorldEvent->ExitWorld(*this, *Object.get());
				}
				return Object->bDestroy;
			});

		WorldObjects.erase(removeIter, WorldObjects.end());

		bFlagDestroyedWorldObject = false;
	}

	template <typename T>
	T* NewObject(CObject* InOwner = nullptr)
	{
		T* Object = new T;
		Object->InstanceId = NumberGenerator.GenerateNumber();
		Object->World = this;

		if (InOwner)
			InOwner->AttachChild(Object);

		NextAddedWorldObjects.push(Object);
		
		return Object;
	}

	const std::vector<std::unique_ptr<CObject>>& GetWorldObjects() const { return WorldObjects; }
	void MarkDestroyed() { bFlagDestroyedWorldObject = true; }
	void AddObjectEnterWorldEvent(std::unique_ptr<IObjectEnterWorld> InObjectEnterWorldEvent)
	{
		ObjectEnterWorldEvents.push_back(std::move(InObjectEnterWorldEvent));
	}
	void AddObjectExitWorldEvent(std::unique_ptr<IObjectExitWorld> InObjectExitWorldEvent)
	{
		ObjectExitWorldEvents.push_back(std::move(InObjectExitWorldEvent));
	}

private:
	std::vector<std::unique_ptr<CObject>> WorldObjects;
	std::queue<CObject*> NextAddedWorldObjects;
	bool bFlagDestroyedWorldObject;

	CNumberGenerator NumberGenerator;

	std::vector<std::unique_ptr<IObjectEnterWorld>> ObjectEnterWorldEvents;
	std::vector<std::unique_ptr<IObjectExitWorld>> ObjectExitWorldEvents;
};