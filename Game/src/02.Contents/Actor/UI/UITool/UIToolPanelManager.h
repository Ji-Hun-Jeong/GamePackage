#pragma once
#include "01.Base/Object/Actor.h"
#include "UIToolStates.h"
#include "UIToolPanel.h"

class CUIToolPanelManager : public CActor
{
	GENERATE_OBJECT()
public:
	CUIToolPanelManager() = default;
	~CUIToolPanelManager() = default;

public:
	void Destroy() override
	{
		CActor::Destroy();
		for (auto& UIToolState : UIToolStates)
			UIToolState->Destroy();
	}
	void InitUIToolPanelManager(CUIToolPanel& InUIToolPanel)
	{
		UIToolPanel = &InUIToolPanel;
		assert(UIToolPanel);

		UIToolPanel->GetInteractionComponent()->SetMouseEnterEvent([this](const Vector2& InMousePosition)->void
			{
				CurrentUIToolState->MouseEnter(InMousePosition);
			});
		UIToolPanel->GetInteractionComponent()->SetMouseExitEvent([this](const Vector2& InMousePosition)->void
			{
				CurrentUIToolState->MouseExit(InMousePosition);
			});
		UIToolPanel->GetInteractionComponent()->SetMouseMoveEvent([this](const Vector2& InMousePosition)->void
			{
				CurrentUIToolState->MouseMove(InMousePosition);
			});
		UIToolPanel->GetInteractionComponent()->SetMouseClickEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				CurrentUIToolState->MouseClick(InKeyType, InMousePosition);
			});
		UIToolPanel->GetInteractionComponent()->SetMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				CurrentUIToolState->MouseRelease(InKeyType, InMousePosition);
			});
	}
	void AddUIToolState(IUIToolState* InUIToolState)
	{
		assert(InUIToolState);
		InUIToolState->SetUIToolPanel(UIToolPanel);
		UIToolStates.push_back(InUIToolState);
	}
	void SetCurrentUIToolState(IUIToolState* InUIToolState)
	{
		assert(InUIToolState);
		if (CurrentUIToolState)
			CurrentUIToolState->ExitState();
		CurrentUIToolState = InUIToolState;
		CurrentUIToolState->EnterState();
	}

private:
	CUIToolPanel* UIToolPanel;

	std::vector<IUIToolState*> UIToolStates;
	IUIToolState* CurrentUIToolState;

};

