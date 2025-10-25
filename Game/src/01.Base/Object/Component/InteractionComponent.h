#pragma once
#include "Component.h"
#include "06.Interaction/MouseManager.h"

class CInteractionComponent : public CComponent
{
	DONTCOPY(CInteractionComponent)
	GENERATE_OBJECT()
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
	}
	void SetRectTransform(float InPositionX, float InPositionY, float InScaleX, float InScaleY)
	{
		MouseInteracter->SetPosition(Vector2(InPositionX, InPositionY));
		MouseInteracter->SetSize(Vector2(InScaleX, InScaleY));
	}
	void SetMouseManager(CMouseManager* InMouseManager);

	void PerformEvent()
	{
		const Vector2& MousePosition = MouseManager->GetMousePosition();
		bool bMouseMove = MouseManager->IsMouseMove();
		bool bMouseClicked = MouseManager->IsMouseClicked();
		bool bMouseHolded= MouseManager->IsMouseHolded();
		bool bMouseReleased = MouseManager->IsMouseReleased();
		EKeyType KeyType = MouseManager->GetKeyType();

		if (MouseInteracter->IsMouseEnter() && MouseEnterEvent)
			MouseEnterEvent(MousePosition);

		if (MouseInteracter->IsMouseExit() && MouseExitEvent)
			MouseExitEvent(MousePosition);

		if (MouseInteracter->IsMouseOn())
		{
			if (MouseOnEvent)
				MouseOnEvent(MousePosition);
			if (MouseOnMoveEvent && bMouseMove)
				MouseOnMoveEvent(MousePosition);
			if (MouseOnClickEvent && bMouseClicked)
				MouseOnClickEvent(KeyType, MousePosition);
			if (MouseOnHoldedEvent && bMouseHolded)
				MouseOnHoldedEvent(KeyType, MousePosition);
			if (MouseOnReleaseEvent && bMouseReleased)
				MouseOnReleaseEvent(KeyType, MousePosition);
		}

		if (MouseInteracter->IsMouseFocus())
		{
			if (MouseFocusEvent)
				MouseFocusEvent(MousePosition);
			if (MouseFocusMoveEvent && bMouseMove)
				MouseFocusMoveEvent(MousePosition);
			if (MouseFocusClickEvent && bMouseClicked)
				MouseFocusClickEvent(KeyType, MousePosition);
			if (MouseFocusHoldedEvent && bMouseHolded)
				MouseFocusHoldedEvent(KeyType, MousePosition);
			if (MouseFocusReleaseEvent && bMouseReleased)
				MouseFocusReleaseEvent(KeyType, MousePosition);
		}

	}

	bool IsInteracterOn() const { return MouseInteracter->IsMouseOn(); }
	bool IsInteracterFocus() const { return MouseInteracter->IsMouseFocus(); }
	bool IsMouseClicked() const { return MouseManager->IsMouseClicked(); }
	bool IsMouseHolded() const { return MouseManager->IsMouseHolded(); }
	bool IsMouseReleased() const { return MouseManager->IsMouseReleased(); }
	bool IsMouseMove() const { return MouseManager->IsMouseMove(); }
	EKeyType GetMouseType() const { return MouseManager->GetKeyType(); }
	const Vector2& GetMousePosition() const { return MouseManager->GetMousePosition(); }


	void SetInteracterActivateEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetInteracterDeactivateEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }

	// On Events
	void SetInteracterOnMouseMoveEvent(std::function<void(const Vector2&)> InMouseOnMoveEvent) { MouseOnMoveEvent = InMouseOnMoveEvent; }
	void SetInteracterOnMouseClickEvent(std::function<void(EKeyType, const Vector2&)> InMouseOnClickEvent) { MouseOnClickEvent = InMouseOnClickEvent; }
	void SetInteracterOnMouseHoldedEvent(std::function<void(EKeyType, const Vector2&)> InMouseOnHoldedEvent) { MouseOnHoldedEvent = InMouseOnHoldedEvent; }
	void SetInteracterOnMouseReleaseEvent(std::function<void(EKeyType, const Vector2&)> InMouseOnReleaseEvent) { MouseOnReleaseEvent = InMouseOnReleaseEvent; }

	// Focus Events
	void SetInteracterFocusMouseMoveEvent(std::function<void(const Vector2&)> InMouseFocusMoveEvent) { MouseFocusMoveEvent = InMouseFocusMoveEvent; }
	void SetInteracterFocusMouseClickEvent(std::function<void(EKeyType, const Vector2&)> InMouseFocusClickEvent) { MouseFocusClickEvent = InMouseFocusClickEvent; }
	void SetInteracterFocusMouseHoldedEvent(std::function<void(EKeyType, const Vector2&)> InMouseFocusHoldedEvent) { MouseFocusHoldedEvent = InMouseFocusHoldedEvent; }
	void SetInteracterFocusMouseReleaseEvent(std::function<void(EKeyType, const Vector2&)> InMouseFocusReleaseEvent) { MouseFocusReleaseEvent = InMouseFocusReleaseEvent; }

private:
	CMouseManager* MouseManager;
	CMouseInteracter* MouseInteracter;

	std::function<void(const Vector2&)> MouseEnterEvent;
	std::function<void(const Vector2&)> MouseExitEvent;
	std::function<void(const Vector2&)> MouseOnEvent;
	std::function<void(const Vector2&)> MouseFocusEvent;

	std::function<void(const Vector2&)> MouseOnMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseOnClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseOnHoldedEvent;
	std::function<void(EKeyType, const Vector2&)> MouseOnReleaseEvent;

	std::function<void(const Vector2&)> MouseFocusMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseFocusClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseFocusHoldedEvent;
	std::function<void(EKeyType, const Vector2&)> MouseFocusReleaseEvent;

};

