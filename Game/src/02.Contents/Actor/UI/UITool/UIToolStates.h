#pragma once
#include <stack>
#include "01.Base/Actor/UI.h"
#include "UIToolPanel.h"

class IUIToolState
{
public:
	IUIToolState() {}
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
	void PlaceUIOnMouseReleased(CUIToolPanel& InUIToolPanel);
	void DecideOwnerUI(CUI* InFocusUI);

private:
	CUI& LoadDialogUI;
	class CWindowIOManager& WindowIOManager;
	class CMousePointer& MousePointer;

	std::stack<CUI*> UIStack;

	CUI* CurrentFocusUI;
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
		InUIToolPanel.SetChangeFocusUIEvent([this](CUIToolPanel& InUIToolPanel, CUI* InCurrentFocusUI, const Vector2& InMousePosition)->void
			{
				GetCurrentFocusUI(InUIToolPanel, InCurrentFocusUI);
			});
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseClickEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				GetCurrentMovedUIFromFocusUI(InMousePosition);
			});
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				ReleaseMovedUI();
			});
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseMoveEvent([this](const Vector2& InMousePosition)->void
			{
				MoveCurrentMovedUI(InMousePosition);
			});
	}
	void ExitState(CUIToolPanel& InUIToolPanel) override
	{
		InUIToolPanel.SetChangeFocusUIEvent(nullptr);
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseClickEvent(nullptr);
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseReleaseEvent(nullptr);
		InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseMoveEvent(nullptr);
	}

	void GetCurrentFocusUI(CUIToolPanel& InUIToolPanel, CUI* InCurrentFocusUI)
	{
		if (&InUIToolPanel != InCurrentFocusUI)
		{
			CurrentFocusUIOnUIToolPanel = InCurrentFocusUI;
			std::cout << CurrentFocusUIOnUIToolPanel << std::endl;
		}
	}
	void GetCurrentMovedUIFromFocusUI(const Vector2& InMousePosition)
	{
		if (CurrentFocusUIOnUIToolPanel)
		{
			CurrentMovedUI = CurrentFocusUIOnUIToolPanel;
			MouseFocusUIDiff = InMousePosition - Vector2(CurrentMovedUI->GetTransform()->GetFinalPosition().x
				, CurrentMovedUI->GetTransform()->GetFinalPosition().y);
		}
	}
	void ReleaseMovedUI()
	{
		if (CurrentMovedUI)
			CurrentMovedUI = nullptr;
	}

	void MoveCurrentMovedUI(const Vector2& InMousePosition)
	{
		if (CurrentMovedUI == nullptr)
			return;
		Vector3 NewPosition = Vector3(InMousePosition.x, InMousePosition.y, CurrentMovedUI->GetTransform()->GetPosition().z) - MouseFocusUIDiff;
		CurrentMovedUI->GetTransform()->SetPosition(NewPosition - CurrentMovedUI->GetOwner()->GetTransform()->GetFinalPosition());
	}

private:
	CUI* CurrentFocusUIOnUIToolPanel = nullptr;
	CUI* CurrentMovedUI = nullptr;
	Vector2 MouseFocusUIDiff;

};