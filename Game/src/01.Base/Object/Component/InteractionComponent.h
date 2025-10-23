#pragma once
#include "Component.h"
#include "06.Interaction/MouseManager.h"

class CInteractionComponent : public CComponent
{
	GENERATE_OBJECT()
	DONTCOPY(CInteractionComponent)
public:
	CInteractionComponent()
		: MouseManager(nullptr)
		, MouseInteracter(nullptr)
	{}
	~CInteractionComponent()
	{

	}

public:
	void EndPlay() override
	{
		CComponent::EndPlay();
		MouseInteracter->Destroy();
		for (auto& ChildInteractionComponent : ChildInteractionComponents)
			ChildInteractionComponent->Destroy();
	}
	void SetRectTransform(float InPositionX, float InPositionY, float InScaleX, float InScaleY)
	{
		MouseInteracter->SetPosition(Vector2(InPositionX, InPositionY));
		MouseInteracter->SetSize(Vector2(InScaleX, InScaleY));
	}
	void SetMouseManager(CMouseManager* InMouseManager);
	void SetMouseEnterEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseInteracter->SetMouseEnterEvent(InMouseEnterEvent); }
	void SetMouseExitEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseInteracter->SetMouseExitEvent(InMouseExitEvent); }
	void SetMouseClickEvent(std::function<void(EKeyType, const Vector2&)> InMouseClickEvent) { MouseInteracter->SetMouseClickEvent(InMouseClickEvent); }
	void SetMouseReleaseEvent(std::function<void(EKeyType, const Vector2&)> InMouseReleaseEvent) { MouseInteracter->SetMouseReleaseEvent(InMouseReleaseEvent); }
	void SetMouseMoveEvent(std::function<void(const Vector2&)> InMouseMoveEvent) { MouseInteracter->SetMouseMoveEvent(InMouseMoveEvent); }
	void AddChildInteractionComponent(CInteractionComponent* InChildInteractionComponent)
	{
		MouseInteracter->AttachChildInteracter(InChildInteractionComponent->MouseInteracter);
		ChildInteractionComponents.push_back(InChildInteractionComponent);
	}

private:
	CMouseManager* MouseManager;
	CMouseInteracter* MouseInteracter;
	std::vector<CInteractionComponent*> ChildInteractionComponents;

};

