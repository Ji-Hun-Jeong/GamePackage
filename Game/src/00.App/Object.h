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
		, bDestroy(false)
	{
	}
	virtual ~CObject() = 0
	{
		for (auto& DestroyEvent : ObjectDestroyEvents)
			(*DestroyEvent)();
	}

private:
	UINT InstanceId;
	bool bDestroy;

public:
	UINT GetInstanceId() const { return InstanceId; }
	bool IsDestroy() const { return bDestroy; }

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
