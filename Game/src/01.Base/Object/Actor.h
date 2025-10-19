#pragma once
#include "Object.h"
#include "Component/Transform.h"
#include "Component/RenderComponent.h"
#include "Component/InteractionComponent.h"
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
	CInteractionComponent* GetInteractionComponent() const { return InteractionComponent.get(); }
	template <typename T>
	T* GetComponent()
	{
		for (auto& Component : Components)
		{
			if (Component->GetType() == T::GetStaticType())
				return Component;
		}
	}
	void AddComponent(CComponent* InComponent)
	{
		Components.push_back(InComponent);
	}

private:
	std::unique_ptr<CTransform> Transform;
	std::unique_ptr<CRenderComponent> RenderComponent;
	std::unique_ptr<CAnimator> Animator;
	std::unique_ptr<CInteractionComponent> InteractionComponent;
	std::vector<CComponent*> Components;

public:
	void SetRenderComponent();
	void SetInteractionComponent();
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

			if (InteractionComponent)
			{
				InteractionComponent->SetRectTransform(Transform->GetFinalPosition().x, Transform->GetFinalPosition().y
					, Transform->GetFinalScale().x * 2.0f, Transform->GetFinalScale().y * 2.0f);
			}
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

		if (Transform)
			Transform->Destroy();
		if (RenderComponent)
			RenderComponent->Destroy();
		if (Animator)
			Animator->Destroy();
		if (InteractionComponent)
			InteractionComponent->Destroy();
		for (auto& Component : Components)
			Component->Destroy();
	}

};

