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

			if (Object->GetOwner() == nullptr)
				WorldObjects.emplace_back(Object);
		}
		for (auto& WorldObject : WorldObjects)
			WorldObject->Update(0.5f);
	}
	void Render()
	{
		
	}
	template <typename T>
	T* NewObject(CObject* InOwner = nullptr)
	{
		T* Object = new T;
		Object->SetInstanceId(NumberGenerator.GenerateNumber());
		Object->World = this;

		if (InOwner)
			InOwner->AttachChild(std::unique_ptr<CObject>(Object));

		NextAddedWorldObjects.push(Object);
		
		return Object;
	}

private:
	CWorldEventHandler WorldEventHandler;

	std::list<std::unique_ptr<CObject>> WorldObjects;
	std::queue<CObject*> NextAddedWorldObjects;

	CNumberGenerator NumberGenerator;
	
};