#pragma once
#include "ObjectPtr.h"
#include "Class.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CCoreSystem
{
	SINGLE(CCoreSystem)
public:
	~CCoreSystem()
	{
		
	}
	void ArrangeObjects()
	{
		for (auto Iter = Objects.begin(); Iter != Objects.end();)
		{
			CObject* Object = *Iter;
			if (Object->bDestroy)
			{
				NumberGenerator.ReleaseNumber(Object->InstanceId);

				Iter = Objects.erase(Iter);
				delete Object;
			}
			else
				++Iter;
		}

		while (DeletedObjects.empty() == false)
		{
			CObject* Object = DeletedObjects.front();
			DeletedObjects.pop();

			Object->bDestroy = true;			
		}
	}
	void RegistObject(CObject* InObject)
	{
		InObject->InstanceId = NumberGenerator.GenerateNumber();
		Objects.push_back(InObject);
	}
	void DeregistObject(CObject* InObject)
	{
		DeletedObjects.push(InObject);
	}
	void ShutDown()
	{
		for (auto& Object : Objects)
			delete Object;
	}
private:
	std::vector<CObject*> Objects;
	std::queue<CObject*> DeletedObjects;

	CNumberGenerator NumberGenerator;
};

template <typename T>
T* NewObject(CObject* InOwner = nullptr, CClass* InClass = nullptr)
{
	T* Object = nullptr;
	if (InClass)
		Object = static_cast<T*>(InClass->CreateObject());
	else
		Object = new T;

	assert(Object);

	CCoreSystem::GetInst().RegistObject(Object);
	return Object;
}

void DestroyObject(CObject* InObject);

template <typename T>
bool IsSame(CObject& InCompareObject)
{
	return InCompareObject.GetType() == T::GetStaticType();
}