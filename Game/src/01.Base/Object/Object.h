#pragma once
#include "../Component/RenderComponent.h"
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
	virtual ~CObject()
	{

	}

private:
	friend class CWorld;
	class CWorld* World;
	UINT InstanceId;
	CObject* Owner;

	std::vector<CObject*> Childs;
	bool bDestroy;

	void AttachChild(CObject* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
	}
	void DetachChild(CObject* InChild)
	{
		for (auto Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
		{
			if ((*Iter) == InChild)
			{
				Childs.erase(Iter);
				break;
			}
		}
	}

protected:
	class CWorld* GetWorld() { return World; }
	CObject* GetOwner() { return Owner; }

public:
	UINT GetInstanceId() const { return InstanceId; }

public:
	CRenderComponent* GetRenderComponent() const { return RenderComponent.get(); }

protected:
	std::unique_ptr<CRenderComponent> RenderComponent;

protected:
	CObject()
		: InstanceId(0)
		, World(nullptr)
		, Owner(nullptr)
		, RenderComponent(nullptr)
		, bDestroy(false)
	{
	}

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
		for (auto& ObjectDestroy : ObjectDestroyEvents)
			ObjectDestroy->OnDestroy(*this);
	}

public:
	// ObjectDestroy
	// 표시해놓고 순회할때 지우는걸로 Render같은 때에
	void Destroy();
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