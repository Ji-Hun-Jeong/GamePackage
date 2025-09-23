#pragma once

class IObjectDestroy
{
	friend class CObject;
	virtual void OnDestroy(class CObject& InObject) = 0;
};

class CObject
{
	friend class CWorld;
	DONTASSN(CObject)
public:
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, Owner(nullptr)
	{
	}

	virtual ~CObject()
	{
		for (auto& ObjectDestroy : ObjectDestroyEvents)
			ObjectDestroy->OnDestroy(*this);
	}

protected:
	CObject(const CObject& InOther);
	CObject(CObject&& InOther) noexcept;

public:
	virtual CObject* Clone() = 0;
	virtual void Update(float InDeltaTime)
	{
		for (auto& Child : Childs)
			Child->Update(InDeltaTime);
	}

private:
	// Setting From World
	void SetWorld(class CWorld* InWorld);
	void SetInstanceId(class CNumberGenerator& InNumberGenerator);
	void SetOwner(CObject* InOwner);

	class CWorld* World;

public:
	UINT GetInstanceId() const { return InstanceId; }

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
	UINT InstanceId;

	std::vector<std::unique_ptr<IObjectDestroy>> ObjectDestroyEvents;

protected:
	CObject* Owner;
	std::vector<std::unique_ptr<CObject>> Childs;


};

