#pragma once
#include "01.Base/World/World.h"
#include "03.Utils/InstancePool.h"

template <typename T>
extern T* NewObject(class CActor* InOwnerActor);

template <typename T>
extern T* SpawnActor(class CActor* InOwnerActor = nullptr);

template <typename T_SCENE>
extern void LoadScene();

class CObject
{
	DONTCOPY(CObject)
public:
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, bDestroy(false)
	{
	}
	virtual ~CObject() = 0
	{
	}

private:
	friend class CWorld;
	CWorld* World;
	UINT InstanceId;
	bool bDestroy;

protected:
	CWorld* GetWorld() { return World; }
	virtual void ResetOwner(class CActor* InOwnerActor) = 0;
	virtual void SetOwner(class CActor* InOwnerActor) = 0;
	virtual class CActor* GetOwner() const = 0;
public:
	UINT GetInstanceId() const { return InstanceId; }

protected:


public:
	virtual void BeginPlay()
	{
		for (auto& BeginEvent : BeginEvents)
			(*BeginEvent)(*this);
	}
	virtual void EndPlay()
	{
		for (auto& EndEvent : EndEvents)
			(*EndEvent)(*this);
	}
	virtual void Initalize() = 0;
	virtual void Destroy() = 0;
	virtual void Reset()
	{
		EndEvents.clear();
		World = nullptr;
	}

	virtual void SetInputAction(class CInputActionManager& InInputActionManager) {}

	virtual void Serialize(CSerializer& InSerializer) const = 0;
	virtual void Deserialize(const CSerializer& InDeserializer) = 0;

public:
	void AddEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.insert(&InEndEvent); }
	void AddBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.insert(&InBeginEvent); }
	void RemoveEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.erase(&InEndEvent); }
	void RemoveBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.erase(&InBeginEvent); }

private:
	std::set<std::function<void(CObject&)>*> EndEvents;
	std::set<std::function<void(CObject&)>*> BeginEvents;

public:
	virtual ObjectType GetType() const = 0;
	virtual class CClass* GetClass() const = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

protected:

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
