#include "pch.h"
#include "UIToolStates.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Base/WindowIOManager.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "UIToolPanel.h"

CUI* CUIToolInputState::DoPlaceUI(const Vector3& InPosition, const std::wstring& InImagePath, CUIToolPanel& InUIToolPanel)
{
	CUI* NewUI = GetWorld()->SpawnActor<CUI>(&InUIToolPanel);
	NewUI->GetTransform()->SetPosition(InPosition);
	if (InImagePath.empty() == false)
		NewUI->GetRenderComponent()->SetDiffuseImage(InImagePath);
	UIs.push(NewUI);

	return NewUI;
}

void CUIToolInputState::UndoPlaceUI()
{
	if (UIs.empty())
		return;
	UIs.top()->Destroy();
	UIs.pop();
}

void CUIToolInputState::PlaceUIOnToolPanel(EKeyType InKeyType, const std::wstring& InUIImage, const Vector2& InMouseWorldPosition, CUIToolPanel& InUIToolPanel)
{
	if (InKeyType == EKeyType::LButton)
	{
		if (InUIImage.empty())
			return;

		Vector3 Position = Vector3(InMouseWorldPosition.x, InMouseWorldPosition.y, 0.0f);
		DoPlaceUI(Position, InUIImage, InUIToolPanel);
	}
	if (InKeyType == EKeyType::RButton)
	{
		UndoPlaceUI();
		// MousePointer->GetRenderComponent()->ResetImage();
	}
}
