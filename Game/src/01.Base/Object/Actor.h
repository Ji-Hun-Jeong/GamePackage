#pragma once
#include "Object.h"
#include "Component/Transform.h"
#include "Component/RenderComponent.h"
#include "Component/Animation/Animator.h"

class CActor : public CObject
{
	GENERATE_OBJECT()
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
	CAnimator* GetAnimator() const { return Animator.get(); }

private:
	std::unique_ptr<CTransform> Transform;
	std::unique_ptr<CRenderComponent> RenderComponent;
	std::unique_ptr<CAnimator> Animator;

public:
	void SetRenderComponent();
	void SetAnimator();

protected:
	virtual void Initalize();
	virtual void BeginPlay() override
	{
		CObject::BeginPlay();

	}
	virtual void Update(float InDeltaTime)
	{
		if (Animator)
		{
			bool bChangeAnimation = Animator->TryChangeCurrentAnimation();

			CAnimation* CurrentAnimation = Animator->GetCurrentAnimation();
			if (bChangeAnimation)
				CurrentAnimation->RequestFrame(0);

			bool bChangeFrame = CurrentAnimation->TryChangeFrame();
			if (bChangeFrame)
			{
				const TFrame& ChangedFrame = CurrentAnimation->GetCurrentFrame();

				if (ChangedFrame.ImagePath.empty() == false)
					RenderComponent->SetImage(ChangedFrame.ImagePath);
			}

			CurrentAnimation->UpdateAnimationState(InDeltaTime);
		}
	}
	virtual void FinalUpdate()
	{
		if (Owner)
			Transform->SetFinalPosition(Transform->GetPosition() + Owner->GetTransform()->GetFinalPosition());
		else
			Transform->SetFinalPosition(Transform->GetPosition());
	}
	virtual void CaptureSnapShot()
	{
		if (Transform->OnVariation())
		{
			Transform->CalculateModelMatrix();
			Transform->SetVariation(false);
			if (RenderComponent)
				RenderComponent->UpdateVertexConstBuffer(0, &Transform->GetModelMatrix(), sizeof(Transform->GetModelMatrix()));
		}
		for (auto& Child : Childs)
			Child->GetTransform()->SetVariation(true);
	}
	virtual void Serialize(CSerializer& InSerializer) const override
	{
		CObject::Serialize(InSerializer);
		CSerializer ComponentArray = CSerializer::array();
		auto Save = [this, &ComponentArray](CComponent* InComponent)->void
			{
				CSerializer ComponentData;
				InComponent->Serialize(ComponentData);
				ComponentArray.push_back(ComponentData);
			};
		if (Transform)
			Save(Transform.get());
		if (RenderComponent)
			Save(RenderComponent.get());
		if (Animator)
			Save(Animator.get());
		InSerializer["component"] = ComponentArray;
		for (auto& Child : Childs)
			Child->Serialize(InSerializer);
	}

public:
	virtual void Destroy() override
	{
		CObject::Destroy();

		if (Owner)
			Owner->DetachChild(this);
		for (auto& Child : Childs)
			Child->Destroy();
	}

};

