#pragma once
#include "../Object.h"
#include "../Component/Transform.h"
#include "../Component/RenderComponent.h"

class CActor : public CObject
{
	DONTCOPY(CActor)
public:
	CActor()
		: Owner(nullptr)
		, Transform(nullptr)
		, RenderComponent(nullptr)
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
	CTransform* GetTransform() const { return Transform.get(); }
	CRenderComponent* GetRenderComponent() const { return RenderComponent.get(); }

protected:
	std::unique_ptr<CTransform> Transform;
	std::unique_ptr<CRenderComponent> RenderComponent;

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

