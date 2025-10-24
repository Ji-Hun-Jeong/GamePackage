#include "pch.h"
#include "UIToolStates.h"
#include "01.Base/World/World.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"


CUIToolInputState::CUIToolInputState(CUI& InLoadDialogUI, CWindowIOManager& InWindowIOManager, CMousePointer& InMousePointer)
	: LoadDialogUI(InLoadDialogUI)
	, WindowIOManager(InWindowIOManager)
	, MousePointer(InMousePointer)
{}


void CUIToolInputState::EnterState(CUIToolPanel& InUIToolPanel)
{
	LoadDialogUI.GetInteractionComponent()
		->SetInteracterFocusMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (InKeyType == EKeyType::LButton)
					MousePointer.SetMouseImageFromDialog(WindowIOManager);
			});
	InUIToolPanel.SetCurrentFocusUIFoundEvent([this](CUIToolPanel& InUIToolPanel, CUI& InCurrentClickedUI, const Vector2& InMousePosition)->void
		{
			PlaceUIOnMouseReleased(InCurrentClickedUI, InUIToolPanel);
		});
}

void CUIToolInputState::ExitState(CUIToolPanel& InUIToolPanel)
{
	MousePointer.GetRenderComponent()->ResetImage();
	LoadDialogUI.GetInteractionComponent()->SetInteracterFocusMouseReleaseEvent(nullptr);
	InUIToolPanel.SetCurrentFocusUIFoundEvent(nullptr);
}

void CUIToolInputState::PlaceUIOnMouseReleased(CUI& InFocusUI, CUIToolPanel& InUIToolPanel)
{
	if (InFocusUI.GetInteractionComponent()->IsMouseReleased() == false)
		return;
	EKeyType MouseType = InUIToolPanel.GetInteractionComponent()->GetMouseType();

	if (MouseType == EKeyType::LButton)
	{
		const std::wstring& MouseImagePath = MousePointer.GetRenderComponent()->GetCurrentImagePath();
		if (MouseImagePath.empty())
			return;

		CUI* NewUI = InUIToolPanel.PlaceUIOnToolPanel(&InFocusUI, MousePointer.GetRenderComponent()->GetCurrentImagePath()
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
		assert(0);
}
