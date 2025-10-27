#pragma once
#include "00.App/CoreSystem.h"
#include "Component/Transform.h"
#include "Component/RenderComponent.h"
#include "Component/InteractionComponent.h"
#include "Component/Animation/Animator.h"

class CActor : public CObject
{
	GENERATE_OBJECT(CActor)
		DONTCOPY(CActor)
public:
	CActor()
		: Owner(nullptr)
		, bActive(true)
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
	class CWorld* World;

	CActor* Owner;
	bool bActive;

	std::vector<CActor*> Childs;
	void Attach(CActor* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
	}
public:
	void AttachComponent(CComponent* InComponent)
	{
		InComponent->OwnerActor = this;
		Components.emplace_back(InComponent);
	}
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
	void DetachComponent(CComponent* InComponent)
	{
		for (auto Iter = Components.begin(); Iter != Components.end(); ++Iter)
		{
			if (*Iter == InComponent)
			{
				Components.erase(Iter);
				break;
			}
		}
	}

public:
	CActor* GetOwner() const { return Owner; }
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
	std::vector<CComponent*> GetComponentsByName(const std::string& InComponentName)
	{
		std::vector<CComponent*> FoundComponents;
		for (auto& Component : Components)
		{
			CClass* Class = Component->GetClass();
			const std::string& ClassName = Class->GetName();
			if (ClassName == InComponentName)
			{
				FoundComponents.push_back(Component);
			}
		}
		return FoundComponents;
	}

private:
	// 이거 그냥 나중에는 전부 CObjectPtr로 관리
	CTransform* Transform;
	CRenderComponent* RenderComponent;
	CAnimator* Animator;
	CInteractionComponent* InteractionComponent;
	std::vector<CComponent*> Components;

public:
	void SetRenderComponent();
	void SetInteractionComponent();
	void SetAnimator();
public:
	void AddEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.insert(&InEndEvent); }
	void AddBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.insert(&InBeginEvent); }
	void RemoveEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.erase(&InEndEvent); }
	void RemoveBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.erase(&InBeginEvent); }

private:
	std::set<std::function<void(CObject&)>*> EndEvents;
	std::set<std::function<void(CObject&)>*> BeginEvents;

protected:
	virtual void Initalize();
	virtual void BeginPlay()
	{
	}
	virtual void EndPlay()
	{
	}
	virtual void OnCreate() override {}
	virtual void OnDestroy() override {}

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
		CClass* Class = GetClass();
		assert(Class);
		InSerializer["Name"] = Class->GetName();

		if (!InSerializer.contains("Components"))
			InSerializer["Components"] = CSerializer::object();

		CSerializer& ComponentsObject = InSerializer["Components"];

		for (const auto& Component : Components)
		{
			CClass* ComponentClass = Component->GetClass();
			const std::string& ComponentName = ComponentClass->GetName();

			// 해당 이름의 배열이 componentsObject 안에 없으면 빈 배열([])로 생성
			if (!ComponentsObject.contains(ComponentName))
				ComponentsObject[ComponentName] = CSerializer::array();

			CSerializer& ComponentArray = ComponentsObject[ComponentName];

			CSerializer ComponentData;
			Component->Serialize(ComponentData);

			ComponentArray.push_back(ComponentData);
		}

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
		// const auto&로 받아야 수정되지 않음을 보장해요.
		const auto& ComponentsObject = InDeserializer["Components"];

		for (auto& [key, value] : ComponentsObject.items())
		{
			std::vector<CComponent*> MatchComponentArray = GetComponentsByName(key);
			for (size_t i = 0; i < value.size(); ++i)
			{
				if (value[i].is_null())
					continue;
				const CSerializer& ComponentData = value[i];
				MatchComponentArray[i]->Deserialize(ComponentData);
			}
		}

		const auto& ChildArray = InDeserializer["Childs"];
		for (const auto& ChildDataJson : ChildArray)
		{
			const std::string& ClassName = ChildDataJson["Name"];
			CClass* Class = CClassManager::GetInst().GetClassByName(ClassName);
			CActor* Child = Class->CreateObject<CActor>(this);
			//GetWorld()->AppearActor(Child);
			Child->Deserialize(ChildDataJson);
		}
	}

public:
	void Destroy()
	{
		// 월드가 알아서 해줌
		DeleteObject(this);
		for (auto& Child : Childs)
			DeleteObject(Child);
		for (auto& Component : Components)
			DeleteObject(Component);
	}

};

