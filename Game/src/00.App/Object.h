#pragma once

using CSerializer = rapidjson::Value;
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
		if (ObjectDestroyEvent)
			ObjectDestroyEvent();
	}

private:
	UINT InstanceId;
	bool bDestroy;

public:
	UINT GetInstanceId() const { return InstanceId; }
	bool IsDestroy() const { return bDestroy; }

	virtual void Serialize(CSerializer& InSerializer) const {}
	virtual void Deserialize(const CSerializer& InDeserializer) {}
	void SetObjectDestroyEvent(std::function<void()> InObjectDestroyEvent)
	{
		ObjectDestroyEvent = InObjectDestroyEvent;
	}

private:
	std::function<void()> ObjectDestroyEvent;

public:
	virtual ObjectType GetType() const = 0;
	virtual class CClass* GetClass() const = 0;
protected:
	static ObjectType SetType() { return sObjectType++; }
private:
	static ObjectType sObjectType;

};
