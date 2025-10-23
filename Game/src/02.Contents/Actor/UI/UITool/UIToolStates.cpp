#include "pch.h"
#include "UIToolStates.h"
#include "01.Base/World/World.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"

void CUIToolInputState::InitalizeInputState(CUI& InLoadDialogUI, CWindowIOManager& InWindowIOManager, CMousePointer& InMousePointer)
{
	LoadDialogUI = &InLoadDialogUI;
	MousePointer = &InMousePointer;
	WindowIOManager = &InWindowIOManager;
	assert(LoadDialogUI);
	assert(MousePointer);
	assert(WindowIOManager);
}

void CUIToolInputState::MouseRelease(EKeyType InKeyType, const Vector2& InMousePosition)
{
	const std::wstring& MouseImagePath = MousePointer->GetRenderComponent()->GetCurrentImagePath();
	if (InKeyType == EKeyType::LButton)
	{
		if (MouseImagePath.empty())
			return;
		CUI* NewUI = GetUIToolPanel()->PlaceUIOnToolPanel(MouseImagePath, InMousePosition);
		UIStack.push(NewUI);
	}
	if (InKeyType == EKeyType::RButton)
	{
		if (UIStack.empty()) return;
		CUI* LastUI = UIStack.top();
		UIStack.pop();
		LastUI->Destroy();
	}
}

void CUIToolInputState::EnterState()
{
	/*LoadDialogUI->GetInteractionComponent()
		->SetMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				MousePointer->SetMouseImageFromDialog(InKeyType, *WindowIOManager);
			});*/
}

void CUIToolInputState::ExitState()
{
	MousePointer->GetRenderComponent()->ResetImage();
	//LoadDialogUI->GetInteractionComponent()->SetMouseReleaseEvent(nullptr);
}
