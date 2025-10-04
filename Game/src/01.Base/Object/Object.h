#pragma once

class IObjectDestroy
{
	friend class CObject;
	virtual void OnDestroy(class CObject& InObject) = 0;
};

class CObject
{
	DONTCOPY(CObject)
public:
	virtual ~CObject()
	{

	}

private:
	friend class CWorld;
	class CWorld* World;
	UINT InstanceId;
	CObject* Owner;

	// 생명주기는 World가 관여
	std::list<CObject*> Childs;
	bool bDestroy;

	void AttachChild(CObject* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
	}
protected:
	class CWorld* GetWorld() { return World; }
	CObject* GetOwner() { return Owner; }


protected:
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, Owner(nullptr)
		, bDestroy(false)
	{
	}

public:
	virtual void BeginPlay()
	{

	}
	virtual void Update(float InDeltaTime)
	{
		
	}
	virtual void EndPlay()
	{
		std::cout << "EndPlay\n";
	}
	// 표시해놓고 순회할때 지우는걸로 Render같은 때에
	void Destroy()
	{
		bDestroy = true;
		for (auto& Child : Childs)
			Child->Destroy();

		for (auto& ObjectDestroy : ObjectDestroyEvents)
			ObjectDestroy->OnDestroy(*this);
	}
	UINT GetInstanceId() const { return InstanceId; }

public:
	// ObjectDestroy
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

};