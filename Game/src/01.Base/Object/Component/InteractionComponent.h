#pragma once
#include "Component.h"
#include "06.Interaction/MouseInteracter.h"

class CInteractionComponent : public CComponent
{
	GENERATE_OBJECT()
	DONTCOPY(CInteractionComponent)
public:
	CInteractionComponent()
		: MouseInteracter(nullptr)
	{}
	~CInteractionComponent()
	{

	}

public:
	void EndPlay() override
	{
		CComponent::EndPlay();
		MouseInteracter->Destroy();
	}
	void SetRectTransform(float InPositionX, float InPositionY, float InScaleX, float InScaleY)
	{
		MouseInteracter->SetPosition(Vector2(InPositionX, InPositionY));
		MouseInteracter->SetSize(Vector2(InScaleX, InScaleY));
	}
	void SetMouseInteracter(CMouseInteracter* InMouseInteracter) { assert(InMouseInteracter); MouseInteracter = InMouseInteracter; }
	void SetMouseEnterEvent(std::function<void()> InMouseEnterEvent) { MouseInteracter->SetMouseEnterEvent(InMouseEnterEvent); }
	void SetMouseExitEvent(std::function<void()> InMouseExitEvent) { MouseInteracter->SetMouseExitEvent(InMouseExitEvent); }
	void SetMouseClickEvent(std::function<void()> InMouseClickEvent) { MouseInteracter->SetMouseClickEvent(InMouseClickEvent); }
	void SetMouseReleaseEvent(std::function<void()> InMouseReleaseEvent) { MouseInteracter->SetMouseReleaseEvent(InMouseReleaseEvent); }

private:
	CMouseInteracter* MouseInteracter;

};

