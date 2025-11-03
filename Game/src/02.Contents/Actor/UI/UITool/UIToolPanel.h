#pragma once
#include "01.Base/Actor/UI.h"

class CUIToolPanel : public CUI
{
	GENERATE_OBJECT(CUIToolPanel)
public:
	CUIToolPanel() 
		: CurrentFocusUI(nullptr) 
	{
		RenderComponent->SetPSO(EPSOType::Mark);
		PlacedUIs.push_back(this);
	}
	~CUIToolPanel() = default;

public:
	void Update(float InDeltaTime) override
	{
		CUI::Update(InDeltaTime);

		CurrentFocusUI = nullptr;
		for (auto& PlacedUI : PlacedUIs)
		{
			if (PlacedUI->GetInteractionComponent()->IsMouseFocus())
			{
				CurrentFocusUI = PlacedUI;
				break;
			}
		}
	}
	CUI* GetCurrentFocusPlacedUI() const { return CurrentFocusUI; }
	CUI* PlaceUIOnToolPanel(CUI* InOwnerUI, const std::wstring& InUIImagePath, const Vector2& InMouseWorldPosition);
	void EraseUIOnToolPanel(CUI* InErasedUI)
	{
		for (auto Iter = PlacedUIs.begin(); Iter != PlacedUIs.end(); ++Iter)
		{
			CUI* PlacedUI = *Iter;
			if (PlacedUI == InErasedUI)
			{
				PlacedUIs.erase(Iter);
				PlacedUI->Destroy();
				break;
			}
		}
	}

private:
	std::vector<CUI*> PlacedUIs;
	CUI* CurrentFocusUI;

};
