#include "pch.h"
#include "UIToolStates.h"
#include "01.Base/Object/UI.h"

class CUIToolPanel : public CUI
{
	GENERATE_OBJECT()
public:
	CUIToolPanel() = default;
	~CUIToolPanel() = default;

public:
	void Destroy() override
	{
		CUI::Destroy();
		for (auto& UIToolState : UIToolStates)
			UIToolState->Destroy();
	}
	void Initalize() override
	{
		CUI::Initalize();
		GetRenderComponent()->SetPSO(EPSOType::Mark);

		GetInteractionComponent()->SetMouseEnterEvent([this](const Vector2& InMousePosition)->void 
			{
				if (CurrentState->MouseEnterEvent)
					CurrentState->MouseEnterEvent(InMousePosition);
			});
		GetInteractionComponent()->SetMouseExitEvent([this](const Vector2& InMousePosition)->void 
			{
				if (CurrentState->MouseExitEvent)
					CurrentState->MouseExitEvent(InMousePosition);
			});
		GetInteractionComponent()->SetMouseMoveEvent([this](const Vector2& InMousePosition)->void 
			{
				if (CurrentState->MouseMoveEvent)
					CurrentState->MouseMoveEvent(InMousePosition);
			});
		GetInteractionComponent()->SetMouseClickEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (CurrentState->MouseClickEvent)
					CurrentState->MouseClickEvent(InKeyType, InMousePosition);
			});
		GetInteractionComponent()->SetMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (CurrentState->MouseReleaseEvent)
					CurrentState->MouseReleaseEvent(InKeyType, InMousePosition);
			});
	}


	void AddUIToolState(CUIToolState* InUIToolState)
	{
		UIToolStates.push_back(std::move(InUIToolState));
	}

	void SetCurrentState(CUIToolState* InState) { CurrentState = InState; }

private:
	std::vector<CUIToolState*> UIToolStates;
	CUIToolState* CurrentState;

};
