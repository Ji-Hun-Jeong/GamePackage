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
	virtual ObjectType GetType() = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

protected:

};

class CClass
{
public:
	CClass(const std::string& InClassName, std::function<CObject* (class CActor* InOwnerActor)> InCreateFunc);
	~CClass() = default;
public:
	const std::string& GetName() const { return ClassName; }
	CObject* CreateObject(class CActor* InOwnerActor);

private:
	std::string ClassName;
	std::function<CObject* (class CActor* InOwnerActor)> CreateFunc;
};

class CClassManager
{
	SINGLE(CClassManager)
public:
	CClass* GetClass(const std::string& InClassName) const
	{
		auto Iter = Classes.find(InClassName);
		if (Iter == Classes.end())
			return nullptr;
		return Iter->second;
	}
	void AddClass(CClass* InClass)
	{
		Classes.insert({ InClass->GetName(), InClass });
	}
private:
	std::map<std::string, CClass*> Classes;
};

template <typename T>
class CRegister
{
public:
	CRegister(const std::string& InClassName)
	{
		static CClass ClassInstance(InClassName, [](CActor* InOwnerActor)->CObject*
			{
				return NewObject<T>(InOwnerActor);
			});
		CClassManager::GetInst().AddClass(&ClassInstance);
	}
};

#define GENERATE_OBJECT(Class) \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() override {return GetStaticType();};\
private: \
	inline static CRegister<Class> ClassRegister{#Class};\
	
