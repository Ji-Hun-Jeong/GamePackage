#include "pch.h"
#include "UIToolPanel.h"
#include "01.Base/World/World.h"

CUI* CUIToolPanel::PlaceUIOnToolPanel(CUI* InOwnerUI, const std::wstring& InUIImagePath, const Vector2& InMouseWorldPosition)
{
	Vector3 Position = Vector3(InMouseWorldPosition.x, InMouseWorldPosition.y, 0.0f);
	CUI* NewUI = GetWorld()->SpawnActor<CUI>(InOwnerUI);
	NewUI->GetTransform()->SetPosition(Position - InOwnerUI->GetTransform()->GetFinalPosition());
	if (InUIImagePath.empty() == false)
		NewUI->GetRenderComponent()->SetDiffuseImage(InUIImagePath);

	PlacedUIs.push_back(NewUI);

	return NewUI;
}
