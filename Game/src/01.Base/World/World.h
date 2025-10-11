#pragma once
#include "WorldEvent/WorldEventHandler.h"

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

			WorldObjects.emplace_back(Object);
		}

		for (auto& WorldObject : WorldObjects)
			WorldObject->Update(0.5f);

		for (size_t i = 0; i < WorldObjects.size();)
		{
			CObject* Object = WorldObjects[i].get();
			if (Object->bDestroy)
			{
				Object->EndPlay();
				WorldObjects[i] = std::move(WorldObjects.back());
				WorldObjects.pop_back();
			}
			else
				i += 1;
		}
	}
	void Render()
	{
		
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

private:
	std::vector<std::unique_ptr<CObject>> WorldObjects;
	std::queue<CObject*> NextAddedWorldObjects;

	CNumberGenerator NumberGenerator;
	
};