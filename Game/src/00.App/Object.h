#pragma once
#include <nlohmann/json.hpp>
using CSerializer = nlohmann::json;
using ObjectType = size_t;

class CObject
{
	friend class CCoreSystem;
	DONTCOPY(CObject)
public:
	CObject()
		: InstanceId(0)
	{
	}
	virtual ~CObject() = 0
	{
		for (auto& DestroyEvent : ObjectDestroyEvents)
			(*DestroyEvent)();
	}

private:
	UINT InstanceId;


public:
	UINT GetInstanceId() const { return InstanceId; }

	virtual void Serialize(CSerializer& InSerializer) const {}
	virtual void Deserialize(const CSerializer& InDeserializer) {}

public:
	void AddObjectDestroyEvent(std::function<void()>* InObjectDestroyEvent)
	{
		ObjectDestroyEvents.insert(InObjectDestroyEvent);
	}
	void RemoveObjectDestroyEvent(std::function<void()>* InObjectDestroyEvent)
	{
		ObjectDestroyEvents.erase(InObjectDestroyEvent);
	}
private:
	std::set<std::function<void()>*> ObjectDestroyEvents;

public:
	virtual ObjectType GetType() const = 0;
	virtual class CClass* GetClass() const = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

};

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
		ClassInstance = new CClass(InClassName, T::GetStaticType(), [](CActor* InOwnerActor)->CObject*
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
