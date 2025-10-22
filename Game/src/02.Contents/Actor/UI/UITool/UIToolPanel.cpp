#include "pch.h"
#include "UIToolPanel.h"
#include "01.Base/World/World.h"

CUI* CUIToolPanel::PlaceUIOnToolPanel( const std::wstring& InUIImagePath, const Vector2& InMouseWorldPosition)
{
	Vector3 Position = Vector3(InMouseWorldPosition.x, InMouseWorldPosition.y, 0.0f);
	CUI* NewUI = GetWorld()->SpawnActor<CUI>(this);
	NewUI->GetTransform()->SetPosition(Position);
	if (InUIImagePath.empty() == false)
		NewUI->GetRenderComponent()->SetDiffuseImage(InUIImagePath);

	for (auto& CreateChildUIEvent : CreateChildUIEvents)
		CreateChildUIEvent(*NewUI);

	return NewUI;
}
