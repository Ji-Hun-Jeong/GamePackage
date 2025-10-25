#pragma once
#include <nlohmann/json.hpp>
using CSerializer = nlohmann::json;
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

protected:
	class CWorld* GetWorld() { return World; }

public:
	UINT GetInstanceId() const { return InstanceId; }

protected:
	CObject()
		: InstanceId(0)
		, World(nullptr)
	{
	}

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

#define GENERATE_OBJECT() \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() override {return GetStaticType();};


