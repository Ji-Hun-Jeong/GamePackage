#include "pch.h"
#include "UIToolStates.h"

#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"


CUIToolInputState::CUIToolInputState(CUI& InLoadDialogUI, CWindowIOManager& InWindowIOManager, CMousePointer& InMousePointer)
	: LoadDialogUI(InLoadDialogUI)
	, WindowIOManager(InWindowIOManager)
	, MousePointer(InMousePointer)
	, CurrentFocusUI(nullptr)
{}

void CUIToolInputState::DecideOwnerUI(CUI* InFocusUI)
{
	CurrentFocusUI = InFocusUI;
}

void CUIToolInputState::EnterState(CUIToolPanel& InUIToolPanel)
{
	//LoadDialogUI.GetInteractionComponent()
	//	->SetInteracterFocusMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
	//		{
	//			if (InKeyType == EKeyType::LButton)
	//				MousePointer.SetMouseImageFromDialog(WindowIOManager);
	//		});

	//InUIToolPanel.SetChangeFocusUIEvent([this](CUIToolPanel& InUIToolPanel, CUI* InCurrentFocusUI, const Vector2& InMousePosition)->void
	//	{
	//		DecideOwnerUI(InCurrentFocusUI);
	//	});
	//InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseReleaseEvent([this, &InUIToolPanel](EKeyType InKeyType, const Vector2& InMousePosition)->void
	//	{
	//		PlaceUIOnMouseReleased(InUIToolPanel);
	//	});
}

void CUIToolInputState::ExitState(CUIToolPanel& InUIToolPanel)
{
	//MousePointer.GetRenderComponent()->ResetImage();
	//LoadDialogUI.GetInteractionComponent()->SetInteracterFocusMouseReleaseEvent(nullptr);
	//InUIToolPanel.SetChangeFocusUIEvent(nullptr);
	//InUIToolPanel.GetInteractionComponent()->SetInteracterOnMouseReleaseEvent(nullptr);
}

void CUIToolInputState::PlaceUIOnMouseReleased(CUIToolPanel& InUIToolPanel)
{
	if (CurrentFocusUI == nullptr)
		return;

	/*EKeyType MouseType = InUIToolPanel.GetInteractionComponent()->GetMouseType();

	if (MouseType == EKeyType::LButton)
	{
		const std::wstring& MouseImagePath = MousePointer.GetRenderComponent()->GetCurrentImagePath();
		if (MouseImagePath.empty())
			return;

		CUI* NewUI = InUIToolPanel.PlaceUIOnToolPanel(CurrentFocusUI, MousePointer.GetRenderComponent()->GetCurrentImagePath()
			, MousePointer.GetMousePosition());
		NewUI->GetRenderComponent()->SetDiffuseImage(MouseImagePath);
		UIStack.push(NewUI);
	}
	else if (MouseType == EKeyType::RButton)
	{
		if (UIStack.empty())
			return;
		CUI* ErasedUI = UIStack.top();
		UIStack.pop();
		InUIToolPanel.EraseUIOnToolPanel(ErasedUI);
	}
	else
		assert(0);*/
}
