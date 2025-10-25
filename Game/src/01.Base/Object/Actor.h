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
	virtual void Initalize() override;
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
		if (InteractionComponent)
			InteractionComponent->PerformEvent();
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
	virtual void Serialize(CSerializer& InSerializer) const override
	{
		CSerializer ComponentArray = CSerializer::array();
		for (auto& Component : Components)
		{
			CSerializer ComponentData;
			Component->Serialize(ComponentData);
			ComponentArray.push_back(ComponentData);
		}
		InSerializer["Components"] = ComponentArray;

		CSerializer ChildArray = CSerializer::array();
		for (auto& Child : Childs)
		{
			CSerializer ChildData;
			Child->Serialize(ChildData);
			ChildArray.push_back(ChildData);
		}
		InSerializer["Childs"] = ChildArray;
	}
	virtual void Deserialize(const CSerializer& InDeserializer) override
	{
		// "components" 키가 실제로 존재하는지 확인
		if (InDeserializer.contains("components"))
		{
			// const auto&로 받아야 수정되지 않음을 보장해요.
			const auto& ComponentArray = InDeserializer["components"]; // 또는 InDeserializer.at("components");

			// 정말 배열인지 확인하는 것이 좋아요.
			if (ComponentArray.is_array())
			{
				for (const auto& componentDataJson : ComponentArray)
				{
					//matchingComponent->Deserialize(componentDataJson);
				}
			}
		}

		// "childs"도 비슷하게 확인
		if (InDeserializer.contains("childs")) {
			const auto& ChildArray = InDeserializer["childs"];
			if (ChildArray.is_array()) {
				for (const auto& childDataJson : ChildArray) {
					// childDataJson["Type"]을 보고 팩토리로 자식 액터 생성
					// CActor* newChild = CObjectFactory::Get().Create(childDataJson["Type"]);
					// if (newChild) {
					//     newChild->Deserialize(childDataJson);
					//     // Attach...
					// }
				}
			}
		}
	}

public:
	virtual void Destroy() override;

};

