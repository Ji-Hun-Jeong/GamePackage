#pragma once
#include "../Model/AssetLoader.h"

class IObjectDestroy
{
	friend class CObject;
	virtual void OnDestroy(class CObject& InObject) = 0;
};

class CObject
{
	DONTCOPY(CObject)
public:
	virtual ~CObject() = 0
	{

	}

private:
	friend class CWorld;
	class CWorld* World;
	UINT InstanceId;

	bool bDestroy;

protected:
	class CWorld* GetWorld() { return World; }

public:
	UINT GetInstanceId() const { return InstanceId; }

protected:
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, bDestroy(false)
	{
	}

public:
	virtual void BeginPlay()
	{
		
	}
	virtual void EndPlay()
	{
		for (auto& ObjectDestroy : ObjectDestroyEvents)
			ObjectDestroy->OnDestroy(*this);
	}
	virtual void Destroy();

public:
	// ObjectDestroy
	// 표시해놓고 순회할때 지우는걸로 Render같은 때에
	IObjectDestroy* RegistObjectDestroyEvent(std::unique_ptr<IObjectDestroy> InObjectDestroyEvent)
	{
		IObjectDestroy* ObjectDestroy = InObjectDestroyEvent.get();
		ObjectDestroyEvents.push_back(std::move(InObjectDestroyEvent));
		return ObjectDestroy;
	}
	bool RemoveObjectDestroyEvent(IObjectDestroy* InObjectDestroyEvent)
	{
		for (auto Iter = ObjectDestroyEvents.begin(); Iter != ObjectDestroyEvents.end();)
		{
			if ((*Iter).get() == InObjectDestroyEvent)
			{
				ObjectDestroyEvents.erase(Iter);
				return true;
			}
		}
		return false;
	}
private:
	std::vector<std::unique_ptr<IObjectDestroy>> ObjectDestroyEvents;


public:
	virtual size_t GetType() = 0;
	static size_t SetType() { return sObjectType++; }
private:
	static size_t sObjectType;

};

#define GenerateObject() \
public:\
	static size_t GetStaticType()\
	{\
		static size_t ObjectType = SetType();\
		return ObjectType;\
	}\
	size_t GetType() override {return GetStaticType();}