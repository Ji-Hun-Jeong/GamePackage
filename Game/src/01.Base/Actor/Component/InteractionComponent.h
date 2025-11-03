#pragma once
#include "Component.h"
#include "06.Interaction/MouseInteracter.h"

class CInteractionComponent : public CComponent
{
	DONTCOPY(CInteractionComponent)
	GENERATE_OBJECT(CInteractionComponent)
public:
	CInteractionComponent()
	{}
	~CInteractionComponent()
	{

	}

public:
	const CMouseInteracter* GetMouseInteracter() const { return &MouseInteracter; }
	void SetRectTransform(float InPositionX, float InPositionY, float InScaleX, float InScaleY)
	{
		MouseInteracter.SetPosition(Vector2(InPositionX, InPositionY));
		MouseInteracter.SetSize(Vector2(InScaleX, InScaleY));
	}
	void SetMouseEnterEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseInteracter.SetMouseEnterEvent(InMouseEnterEvent); }
	void SetMouseExitEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseInteracter.SetMouseExitEvent(InMouseExitEvent); }
	void SetMouseOnEvent(std::function<void(const Vector2&)> InMouseOnEvent) { MouseInteracter.SetMouseOnEvent(InMouseOnEvent); }
	void SetMouseFocusEvent(std::function<void(const Vector2&)> InMouseFocusEvent) { MouseInteracter.SetMouseFocusEvent(InMouseFocusEvent); }

private:
	CMouseInteracter MouseInteracter;

};

