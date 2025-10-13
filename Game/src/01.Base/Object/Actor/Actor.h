#pragma once
#include "../Object.h"
#include "../Component/RenderComponent.h"

class CActor : public CObject
{
	DONTCOPY(CActor)
public:
	CActor() 
		: Owner(nullptr)
	{}
	virtual ~CActor() {}

private:
	friend class CWorld;
	CActor* Owner;

	std::vector<CActor*> Childs;
	void AttachChild(CActor* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
	}
	void DetachChild(CActor* InChild)
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
public:
	CActor* GetOwner() { return Owner; }
	CRenderComponent* GetRenderComponent() const { return RenderComponent.get(); }

protected:
	std::unique_ptr<CRenderComponent> RenderComponent;

private:
	virtual void Update(float InDeltaTime)
	{
		for (auto& Child : Childs)
			Child->Update(InDeltaTime);
	}

	void Destroy() override
	{
		CObject::Destroy();

		for (auto& Child : Childs)
			Child->Destroy();

		if (Owner)
			Owner->DetachChild(this);
	}

};

