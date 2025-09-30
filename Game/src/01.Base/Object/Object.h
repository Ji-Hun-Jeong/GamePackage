#pragma once

class IObjectDestroy
{
	friend class CObject;
	virtual void OnDestroy(class CObject& InObject) = 0;
};

class CObject
{
	DONTASSN(CObject)
public:
	virtual ~CObject()
	{
		
	}

private:
	friend class CWorld;
	UINT InstanceId;
	CObject* Owner;
	class CWorld* World;
	std::list<std::unique_ptr<CObject>> Childs;
	bool bDestroy;
	void SetInstanceId(UINT InInstanceId) { InstanceId = InInstanceId; }

protected:
	class CWorld* GetWorld() { return World; }
	CObject* GetOwner() { return Owner; }
	void AttachChild(std::unique_ptr<CObject> InChild)
	{
		InChild->Owner = this;
		Childs.push_back(std::move(InChild));
	}

protected:
	// 복사 생성같은거 때문에 생성자는 껍데기역할만 하고 나머지는 세팅해주는 식으로
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, Owner(nullptr)
		, bDestroy(false)
	{
	}
	CObject(const CObject& InOther);
	CObject(CObject&& InOther) noexcept;

public:
	virtual void BeginPlay()
	{

	}
	virtual void Update(float InDeltaTime)
	{
		for (auto& Child : Childs)
			Child->Update(InDeltaTime);
	}
	virtual void EndPlay()
	{
		std::cout << "EndPlay\n";
		for (auto& ObjectDestroy : ObjectDestroyEvents)
			ObjectDestroy->OnDestroy(*this);
		for (auto& Child : Childs)
			Child->EndPlay();
	}

	void Destroy() { bDestroy = true; }
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
	std::vector<std::unique_ptr<IObjectDestroy>> ObjectDestroyEvents;

};