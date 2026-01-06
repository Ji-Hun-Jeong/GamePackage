#pragma once
#include <Core/public/InputManager.h>
#include "00.App/CoreSystem.h"
#include "00.App/WindowManager.h"
#include "00.App/MouseManager.h"
#include "03.Utils/AssetLoader.h"
#include "Component/Transform.h"
#include "Component/RenderComponents/SpriteRenderComponent.h"
#include "Component/PixelCollider.h"
#include "Component/Collider/Collider.h"

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
		if (InChild == nullptr)
			return;
		if (InChild == this)
			return;
		if (InChild->Owner == this)
			return;

		if (InChild->Owner != nullptr)
			InChild->Owner->Detach(InChild);
		
		InChild->Owner = this;
		Childs.push_back(InChild);
	}
	void AttachComponent(CComponent* InComponent);
	void Detach(CActor* InChild)
	{
		if (InChild->Owner != this)
			return;

		for (auto Iter = Childs.begin(); Iter != Childs.end(); ++Iter)
		{
			if (*Iter == InChild)
			{
				Childs.erase(Iter);
				InChild->Owner = nullptr;
				break; 
			}
		}
	}
	void DetachComponent(CComponent* InComponent);

public:
	class CWorld* GetWorld() const { return g_World; }
	CActor* GetOwner() const { return Owner; }

	CTransform* GetTransform() const { return Transform; }
	CSpriteRenderComponent* GetSpriteRenderComponent() const { return SpriteRenderComponent; }
	class CAnimator* GetAnimator() const { return Animator; }

protected:
	// 이거 그냥 나중에는 전부 CObjectPtr로 관리
	CTransform* Transform							= nullptr;
	CSpriteRenderComponent* SpriteRenderComponent	= nullptr;
	class CRigidBody* RigidBody						= nullptr;
	class CAnimator* Animator						= nullptr;
	std::vector<CCollider*> Colliders;
	std::vector<CComponent*> Components;

public:
	template <typename T>
	T* GetComponent()
	{
		// 리플렉션 쓸수가 없으니까 그냥 dynamic_cast ㅜㅜ
		for (auto& Component : Components)
		{
			T* Result = dynamic_cast<T*>(Component);
			if (Result)
				return Result;
		}
	}
	template <typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> FoundComponents;
		FoundComponents.reserve(Components.size());
		for (auto Component : Components)
		{
			T* Result = dynamic_cast<T*>(Component);
			if (Result)
				FoundComponents.push_back(Result);
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
	virtual void OnCollisionEnter(CCollider& InTargetCollider) {}
	virtual void OnCollisionStay(CCollider& InTargetCollider) {}
	virtual void OnCollisionExit(CCollider& InTargetCollider) {}
	virtual void Update(float InDeltaTime);
	virtual void LateUpdate(float InDeltaTime) {} // 충돌이후 로직
	virtual void CaptureSnapShot()
	{

	}
	virtual void RenderActor(class CSpriteRenderer& InRenderer);
	virtual void Activate(bool bInActivate)
	{
		bActive = bInActivate;
		for (auto Child : Childs)
			Child->Activate(bInActivate);
	}
	bool IsActivate() const { return bActive; }

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

