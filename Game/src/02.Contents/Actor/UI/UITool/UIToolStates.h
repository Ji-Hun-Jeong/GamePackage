#pragma once
#include <stack>
#include "01.Base/Object/UI.h"
#include "UIToolPanel.h"

class IUIToolState
{
public:
	IUIToolState(){}
	virtual ~IUIToolState() = 0 {}

public:
	virtual void EnterState(CUIToolPanel& InUIToolPanel) = 0;
	virtual void ExitState(CUIToolPanel& InUIToolPanel) = 0;

};

class CUIToolInputState : public IUIToolState
{
public:
	CUIToolInputState(CUI& InLoadDialogUI, class CWindowIOManager& InWindowIOManager, class CMousePointer& InMousePointer);
	~CUIToolInputState() = default;

public:
	void EnterState(CUIToolPanel& InUIToolPanel) override;
	void ExitState(CUIToolPanel& InUIToolPanel) override;
	void PlaceUIOnMouseReleased(CUI& InFocusUI, CUIToolPanel& InUIToolPanel);

private:
	CUI& LoadDialogUI;
	class CWindowIOManager& WindowIOManager;
	class CMousePointer& MousePointer;

	std::stack<CUI*> UIStack;

};

class CUIToolMoveState : public IUIToolState
{
public:
	CUIToolMoveState()
		: CurrentMovedUI(nullptr)
	{}
	~CUIToolMoveState() {}

public:
	void EnterState(CUIToolPanel& InUIToolPanel) override
	{
		InUIToolPanel.SetCurrentFocusUIFoundEvent([this](CUIToolPanel& InUIToolPanel, CUI& InCurrentFocusUI, const Vector2& InMousePosition)->void
			{
				DecideCurrentMovedUI(InUIToolPanel, InCurrentFocusUI, InMousePosition);
			});
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseMoveEvent([this](const Vector2& InMousePosition)->void
			{
				MoveCurrentMovedUI(InMousePosition);
			});
	}
	void ExitState(CUIToolPanel& InUIToolPanel) override
	{
		InUIToolPanel.SetCurrentFocusUIFoundEvent(nullptr);
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseClickEvent(nullptr);
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseHoldedEvent(nullptr);
	}

	void DecideCurrentMovedUI(CUIToolPanel& InUIToolPanel, CUI& InCurrentFocusUI, const Vector2& InMousePosition)
	{
		if (InUIToolPanel.GetInteractionComponent()->IsMouseClicked() && &InUIToolPanel != &InCurrentFocusUI)
		{
			CurrentMovedUI = &InCurrentFocusUI;
			assert(CurrentMovedUI);
			MouseFocusUIDiff = InMousePosition - Vector2(CurrentMovedUI->GetTransform()->GetFinalPosition().x
				, CurrentMovedUI->GetTransform()->GetFinalPosition().y);
		}
		if (InUIToolPanel.GetInteractionComponent()->IsMouseReleased())
		{
			if (CurrentMovedUI)
				CurrentMovedUI = nullptr;
		}
	}

	void MoveCurrentMovedUI(const Vector2& InMousePosition)
	{
		if (CurrentMovedUI == nullptr)
			return;
		Vector3 NewPosition = Vector3(InMousePosition.x, InMousePosition.y, CurrentMovedUI->GetTransform()->GetPosition().z) - MouseFocusUIDiff;
		CurrentMovedUI->GetTransform()->SetPosition(NewPosition - CurrentMovedUI->GetOwner()->GetTransform()->GetFinalPosition());
	}

private:
	CUI* CurrentMovedUI = nullptr;
	Vector2 MouseFocusUIDiff;

};