#pragma once
#include "00.App/CoreSystem.h"
#include "00.App/WindowManager.h"
#include "03.Utils/AssetLoader.h"
#include "06.Interaction/MouseManager.h"
#include "Component/Transform.h"
#include "Component/RenderComponents/RenderComponent.h"
#include "Component/InteractionComponent.h"
#include "Component/Animation/Animator.h"
#include "Component/PixelCollider.h"
#include "Component/Collider/CollisionManager.h"

extern class CWorld* g_World;

class CActor : public CObject
{
	GENERATE_OBJECT(CActor)
		DONTCOPY(CActor)
public:
	CActor();
	virtual ~CActor()
	{
	}

	const std::vector<CActor*>& GetChild() const { return Childs; }
private:
	CActor* Owner;
	bool bActive;

	std::vector<CActor*> Childs;

public:
	void Attach(CActor* InChild)
	{
		InChild->Owner = this;
		Childs.push_back(InChild);
		if (InteractionComponent && InChild->InteractionComponent)
			InteractionComponent->GetMouseInteracter()->AttachChildInteracter(InChild->InteractionComponent->GetMouseInteracter());
	}
	void AttachComponent(CComponent* InComponent)
	{
		InComponent->OwnerActor = this;
		Components.push_back(InComponent);
	}
	void Detach(CActor* InChild)
	{
		for (auto Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
		{
			if ((*Iter) == InChild)
			{
				if (InteractionComponent && InChild->InteractionComponent)
					InteractionComponent->GetMouseInteracter()->DetachChildInteracter(InChild->InteractionComponent->GetMouseInteracter());
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
	class CWorld* GetWorld() const { return g_World; }
	CActor* GetOwner() const { return Owner; }

	CTransform* GetTransform() const { return Transform; }
	CInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	CRenderComponent* GetRenderComponent() const { return RenderComponent; }

protected:
	// 이거 그냥 나중에는 전부 CObjectPtr로 관리
	CTransform* Transform;
	CRenderComponent* RenderComponent;
	CInteractionComponent* InteractionComponent;
	std::vector<CComponent*> Components;
public:
	template <typename T>
	T* GetComponent()
	{
		for (auto& Component : Components)
		{
			if (Component->GetType() == T::GetStaticType())
				return Component;
		}
	}
	// Move할까 그냥
	template <typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> FoundComponents;
		FoundComponents.reserve(Components.size());
		for (auto Component : Components)
		{
			if (Component->GetType() == T::GetStaticType())
				FoundComponents.push_back(static_cast<T*>(Component));
		}
		return FoundComponents;
	}
	template <typename T>
	T* AddComponent()
	{
		T* Component = NewObject<T>(this);
		AttachComponent(Component);
		return Component;
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
	void AddEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.insert(&InEndEvent); }
	void AddBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.insert(&InBeginEvent); }
	void RemoveEndEvent(std::function<void(CObject&)>& InEndEvent) { EndEvents.erase(&InEndEvent); }
	void RemoveBeginEvent(std::function<void(CObject&)>& InBeginEvent) { BeginEvents.erase(&InBeginEvent); }

private:
	std::set<std::function<void(CObject&)>*> EndEvents;
	std::set<std::function<void(CObject&)>*> BeginEvents;

public:
	virtual void BeginPlay()
	{
	}
	virtual void EndPlay()
	{
	}

	virtual void Update(float InDeltaTime)
	{
		if (InteractionComponent)
			InteractionComponent->PerformEvent();
	}
	virtual void FinalUpdate()
	{
		Vector3 FinalPosition = Transform->GetPosition();
		if (Owner)
			FinalPosition += Owner->Transform->GetFinalPosition();

		Transform->SetFinalPosition(FinalPosition);
	}
	virtual void CaptureSnapShot()
	{
		if (InteractionComponent)
		{
			const Vector3& FinalPosition = Transform->GetFinalPosition();
			InteractionComponent->SetRectPosition(FinalPosition.x, FinalPosition.y);
		}
	}

	virtual void SetupInputActionValue(class CInputActionValueCollector& InInputActionValueCollector) 
	{
	}

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
			//CActor* Child = Class->CreateObject<CActor>(this);
			////GetWorld()->AppearActor(Child);
			//Child->Deserialize(ChildDataJson);
		}
	}

public:
	void Destroy()
	{
		// 월드가 알아서 해줌
		DestroyObject(this);
		for (auto& Child : Childs)
			DestroyObject(Child);
		for (auto& Component : Components)
			DestroyObject(Component);
	}

};

