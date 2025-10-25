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
	{
	}
	virtual ~CObject() = 0
	{
	}
	
private:
	friend class CWorld;
	CWorld* World;
	UINT InstanceId;

protected:
	CWorld* GetWorld() { return World; }
	virtual void SetOwner(class CActor* InOwnerActor) = 0;

public:
	UINT GetInstanceId() const { return InstanceId; }

protected:
	

public:
	virtual void BeginPlay()
	{
		
	}
	virtual void EndPlay()
	{
		for (auto& DestroyEvent : DestroyEvents)
			DestroyEvent(*this);
	}
	virtual void Initalize() = 0;
	virtual void Destroy() = 0;
	virtual void Reset()
	{
		DestroyEvents.clear();
		World = nullptr;
	}

	virtual void SetInputAction(class CInputActionManager& InInputActionManager) {}

	virtual void Serialize(CSerializer& InSerializer) const = 0;
	virtual void Deserialize(const CSerializer& InDeserializer) = 0;

public:
	void AddDestroyEvent(std::function<void(CObject&)> DestroyEvent) { DestroyEvents.push_back(DestroyEvent); }
private:
	std::vector<std::function<void(CObject&)>> DestroyEvents;

public:
	virtual ObjectType GetType() = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

};



#define GENERATE_OBJECT(Class) \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() override {return GetStaticType();};\

