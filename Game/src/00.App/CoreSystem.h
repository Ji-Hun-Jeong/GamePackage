#pragma once
#include "ObjectPtr.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CCoreSystem
{
	SINGLE(CCoreSystem)
public:
	void ArrangeObjects()
	{
		while (DeletedObjects.empty() == false)
		{
			CObject* Object = DeletedObjects.front();
			DeletedObjects.pop();

			NumberGenerator.ReleaseNumber(Object->InstanceId);
			Objects.erase(Object);
			delete Object;
		}
	}
	void RegistObject(CObject* InObject)
	{
		InObject->InstanceId = NumberGenerator.GenerateNumber();
		Objects.insert(InObject);
	}
	void DeregistObject(CObject* InObject)
	{
		DeletedObjects.push(InObject);
	}
private:
	std::set<CObject*> Objects;
	std::queue<CObject*> DeletedObjects;

	CNumberGenerator NumberGenerator;
};

template <typename T>
T* NewObject(CObject* InOwner = nullptr)
{
	T* Object = new T;
	CCoreSystem::GetInst().RegistObject(InObject);
	return Object;
}

void DeleteObject(CObject* InObject)
{
	CCoreSystem::GetInst().DeregistObject(InObject);
}