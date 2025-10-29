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
		for (auto& Object : Objects)
			delete Object;
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
private:
	std::vector<CObject*> Objects;
	std::queue<CObject*> DeletedObjects;

	CNumberGenerator NumberGenerator;
};

template <typename T>
T* NewObject(CObject* InOwner = nullptr, const std::string& InClassName = "")
{
	T* Object = nullptr;
	if (InClassName.empty() == false)
	{
		CClass* Class = CClassManager::GetInst().GetClassByName(InClassName);
		ObjectType Type = Class->GetClassType();
		if (Type == T::GetStaticType())
			Object = static_cast<T*>(Class->CreateObject());
	}
	else
		Object = new T;

	assert(Object);

	CCoreSystem::GetInst().RegistObject(Object);
	return Object;
}

void DestroyObject(CObject* InObject);