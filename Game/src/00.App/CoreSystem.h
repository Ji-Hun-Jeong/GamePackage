#pragma once
#include "ObjectPtr.h"
#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CCoreSystem
{
	SINGLE(CCoreSystem)
public:
	void ArrangeObjects()
	{
		for (auto Iter = Objects.begin(); Iter != Objects.end();)
		{
			CObject* Object = *Iter;
			if (Object->bDestroy)
			{
				NumberGenerator.ReleaseNumber(Object->InstanceId);
				Object->OnDestroy();

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
		InObject->OnCreate();
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
T* NewObject(CObject* InOwner = nullptr)
{
	T* Object = new T;
	CCoreSystem::GetInst().RegistObject(Object);
	return Object;
}

void DeleteObject(CObject* InObject)
{
	CCoreSystem::GetInst().DeregistObject(InObject);
}

class CClass
{
public:
	CClass(const std::string& InClassName, ObjectType InClassType, std::function<CObject* (class CActor* InOwnerActor)> InCreateFunc);
	~CClass() = default;
public:
	ObjectType GetClassType() const { return ClassType; }
	const std::string& GetName() const { return ClassName; }
	template <typename T>
	T* CreateObject(class CActor* InOwnerActor)
	{
		return static_cast<T*>(CreateFunc(InOwnerActor));
	}

private:
	std::string ClassName;
	std::function<CObject* (class CActor* InOwnerActor)> CreateFunc;
	ObjectType ClassType;
};

class CClassManager
{
	SINGLE(CClassManager)
public:
	template <typename T>
	CClass* GetClass() const
	{
		for (auto Iter = Classes.begin(); Iter != Classes.end(); ++Iter)
		{
			if (Iter->second->GetClassType() == T::GetStaticType())
				return Iter->second.get();
		}
		return nullptr;
	}
	CClass* GetClassByName(const std::string& InClassName) const
	{
		auto Iter = Classes.find(InClassName);
		if (Iter == Classes.end())
			return nullptr;
		return Iter->second.get();
	}
	void AddClass(std::unique_ptr<CClass> InClass)
	{
		Classes.insert({ InClass->GetName(), std::move(InClass) });
	}
private:
	std::map<std::string, std::unique_ptr<CClass>> Classes;

};

template <typename T>
class CClassRegister
{
public:
	CClassRegister(const std::string& InClassName)
	{
		// Todo: NewObject에 static class넘기는 방향으로 생각해보기
		ClassInstance = new CClass(InClassName, T::GetStaticType(), [](class CActor* InOwnerActor)->CObject*
			{
				return NewObject<T>(InOwnerActor);
			});
		CClassManager::GetInst().AddClass(std::move(std::unique_ptr<CClass>(ClassInstance)));
	}
public:
	CClass* GetClassInstance() const { return ClassInstance; }

private:
	CClass* ClassInstance;
};

#define GENERATE_OBJECT(Class) \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() const override {return GetStaticType();};\
private: \
	inline static CClassRegister<Class> ClassRegister{#Class};\
public:\
	static CClass* GetStaticClass() {return ClassRegister.GetClassInstance();}\
	CClass* GetClass() const override {return GetStaticClass();};
