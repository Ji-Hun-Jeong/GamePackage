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
		, bDestroy(false)
		, Transform(nullptr)
		, RenderComponent(nullptr)
		, Animator(nullptr)
		, InteractionComponent(nullptr)
	{}
	virtual ~CActor()
	{


	}

	const std::vector<CActor*>& GetChild() const { return Childs; }
private:
	friend class CWorld;
	CActor* Owner;
	bool bDestroy;

	std::vector<CActor*> Childs;
	CActor* Attach(CActor* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
		return InChild;
	}
	CComponent* Attach(CComponent* InComponent)
	{
		InComponent->SetOwner(this);
		Components.emplace_back(InComponent);
		return InComponent;
	}
public:
	void Detach(CActor* InChild)
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
	CTransform* GetTransform() const { return Transform; }
	CRenderComponent* GetRenderComponent() const { return RenderComponent; }
	CAnimator* GetAnimator() const { return Animator; }
	CInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	template <typename T>
	T* GetComponent()
	{
		for (auto& Component : Components)
		{
			if (Component->GetType() == T::GetStaticType())
				return Component;
		}
	}

private:
	CTransform* Transform;
	CRenderComponent* RenderComponent;
	CAnimator* Animator;
	CInteractionComponent* InteractionComponent;
	std::vector<std::unique_ptr<CComponent>> Components;

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
	virtual void EndPlay() override
	{
		CObject::EndPlay();
		for (auto& Component : Components)
			Component->EndPlay();
	}
	void ComponentArrange()
	{
		if (Transform->IsDestroy())
			Transform = nullptr;
		if (RenderComponent && RenderComponent->IsDestroy())
			RenderComponent = nullptr;
		if (Animator && Animator->IsDestroy())
			Animator = nullptr;
		if (InteractionComponent && InteractionComponent->IsDestroy())
			InteractionComponent = nullptr;
		for (auto Iter = Components.begin(); Iter != Components.end();)
		{
			auto& Component = (*Iter);
			if (Component->bDestroy)
				Iter = Components.erase(Iter);
			else
				++Iter;
		}
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
					RenderComponent->SetDiffuseImage(ChangedFrame.ImagePath);
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
			Transform->SetVariation(false);

			if (RenderComponent)
			{
				VertexConstBuffer.ModelMatrix = RenderComponent->GetModelMatrix(Transform->GetFinalPosition(), Transform->GetRotation(), Transform->GetScale());
				RenderComponent->UpdateVertexConstBuffer(0, &VertexConstBuffer, sizeof(VertexConstBuffer));
			}

			if (InteractionComponent)
			{
				InteractionComponent->SetRectTransform(Transform->GetFinalPosition().x, Transform->GetFinalPosition().y
					, Transform->GetScale().x, Transform->GetScale().y);
			}
			for (auto& Child : Childs)
				Child->GetTransform()->SetVariation(true);	
		}
	}
	struct TVertexConstBuffer
	{
		Matrix ModelMatrix;
	} VertexConstBuffer;
	static_assert(sizeof(TVertexConstBuffer) % 16 == 0);

public:
	/*virtual void Serialize(CSerializer& InSerializer) const override
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
	}*/

public:
	virtual void Destroy() override;

};

