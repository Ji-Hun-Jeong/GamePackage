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
	CMouseInteracter* GetMouseInteracter() { return &MouseInteracter; }

	void SetRectTransform(float InPositionX, float InPositionY, float InScaleX, float InScaleY)
	{
		MouseInteracter.SetPosition(Vector2(InPositionX, InPositionY));
		MouseInteracter.SetSize(Vector2(InScaleX, InScaleY));
	}

	void PerformEvent()
	{
		if (MouseInteracter.IsMouseEnter())
			ActivateMouseEnterEvent();
		if (MouseInteracter.IsMouseExit())
			ActivateMouseExitEvent();
		if (MouseInteracter.IsMouseOn())
			ActivateMouseOnEvent();
		if (MouseInteracter.IsMouseFocus())
			ActivateMouseFocusEvent();
	}

	bool IsMouseEnter() const { return MouseInteracter.IsMouseEnter(); }
	bool IsMouseExit() const { return MouseInteracter.IsMouseExit(); }
	bool IsMouseOn() const { return MouseInteracter.IsMouseOn(); }
	bool IsMouseFocus() const { return MouseInteracter.IsMouseFocus(); }

	void SetMouseEnterEvent(std::function<void()> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void()> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseOnEvent(std::function<void()> InMouseOnEvent) { MouseOnEvent = InMouseOnEvent; }
	void SetMouseFocusEvent(std::function<void()> InMouseFocusEvent) { MouseFocusEvent = InMouseFocusEvent; }

private:
	void ActivateMouseEnterEvent() const
	{
		if (MouseEnterEvent)
			MouseEnterEvent();
	}
	void ActivateMouseExitEvent() const
	{
		if (MouseExitEvent)
			MouseExitEvent();
	}
	void ActivateMouseOnEvent() const
	{
		if (MouseOnEvent)
			MouseOnEvent();
	}
	void ActivateMouseFocusEvent() const
	{
		if (MouseFocusEvent)
			MouseFocusEvent();
	}

private:
	CMouseInteracter MouseInteracter;

	std::function<void()> MouseEnterEvent = nullptr;
	std::function<void()> MouseExitEvent = nullptr;
	std::function<void()> MouseOnEvent = nullptr;
	std::function<void()> MouseFocusEvent = nullptr;

};

