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

	void PerformEvent()
	{
		const Vector2& MousePosition = MouseManager->GetMousePosition();
		bool bMouseMove = MouseManager->IsMouseMove();
		bool bMouseClicked = MouseManager->IsMouseClicked();
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
			if (MouseFocusReleaseEvent && bMouseReleased)
				MouseFocusReleaseEvent(KeyType, MousePosition);
		}

	}

	void SetMouseEnterEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseOnEvent(std::function<void(const Vector2&)> InMouseOnEvent) { MouseOnEvent = InMouseOnEvent; }
	void SetMouseFocusEvent(std::function<void(const Vector2&)> InMouseFocusEvent) { MouseFocusEvent = InMouseFocusEvent; }

	void SetMouseOnMoveEvent(std::function<void(const Vector2&)> InMouseOnMoveEvent) { MouseOnMoveEvent = InMouseOnMoveEvent; }
	void SetMouseOnClickEvent(std::function<void(EKeyType, const Vector2&)> InMouseOnClickEvent) { MouseOnClickEvent = InMouseOnClickEvent; }
	void SetMouseOnReleaseEvent(std::function<void(EKeyType, const Vector2&)> InMouseOnReleaseEvent) { MouseOnReleaseEvent = InMouseOnReleaseEvent; }

	void SetMouseFocusMoveEvent(std::function<void(const Vector2&)> InMouseFocusMoveEvent) { MouseFocusMoveEvent = InMouseFocusMoveEvent; }
	void SetMouseFocusClickEvent(std::function<void(EKeyType, const Vector2&)> InMouseFocusClickEvent) { MouseFocusClickEvent = InMouseFocusClickEvent; }
	void SetMouseFocusReleaseEvent(std::function<void(EKeyType, const Vector2&)> InMouseFocusReleaseEvent) { MouseFocusReleaseEvent = InMouseFocusReleaseEvent; }

	void AddChildInteractionComponent(CInteractionComponent* InChildInteractionComponent)
	{
		// Todo: ? 왜 있는거임
		MouseInteracter->AttachChildInteracter(InChildInteractionComponent->MouseInteracter);
		ChildInteractionComponents.push_back(InChildInteractionComponent);
	}

private:
	CMouseManager* MouseManager;
	CMouseInteracter* MouseInteracter;
	std::vector<CInteractionComponent*> ChildInteractionComponents;

	std::function<void(const Vector2&)> MouseEnterEvent;
	std::function<void(const Vector2&)> MouseExitEvent;
	std::function<void(const Vector2&)> MouseOnEvent;
	std::function<void(const Vector2&)> MouseFocusEvent;

	std::function<void(const Vector2&)> MouseOnMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseOnClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseOnReleaseEvent;

	std::function<void(const Vector2&)> MouseFocusMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseFocusClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseFocusReleaseEvent;

};

