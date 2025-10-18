#pragma once
#include <nlohmann/json.hpp>
using CSerializer = nlohmann::json;

class IObjectDestroy
{
	friend class CObject;
	virtual void OnDestroy(class CObject& InObject) = 0;
};

class CObjectCreator
{
	SINGLE(CObjectCreator)
public:

private:

};

using ObjectType = size_t;

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

	virtual void SetInputAction(class CInputActionManager& InInputActionManager) {}

	virtual void Serialize(CSerializer& InSerializer) const {}

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


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	virtual ObjectType GetType() = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

};

#define GENERATE_OBJECT() \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() override {return GetStaticType();};