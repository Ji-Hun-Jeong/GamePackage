#pragma once
#include "pch.h"
#include "01.Base/Object/UI.h"

class CUIToolPanel : public CUI
{
	GENERATE_OBJECT()
public:
	CUIToolPanel() : CurrentFocusUI(nullptr) {}
	~CUIToolPanel() {}

public:
	void Initalize() override
	{
		CUI::Initalize();
		GetRenderComponent()->SetPSO(EPSOType::Mark);
		PlacedUIs.push_back(this);
	}

public:
	void Update(float InDeltaTime) override
	{
		CUI::Update(InDeltaTime);

		CUI* NewFocusUI = nullptr;
		for (auto& PlacedUI : PlacedUIs)
		{
			if (PlacedUI->GetInteractionComponent()->IsInteracterFocus())
			{
				NewFocusUI = PlacedUI;
				break;
			}
		}

		if (CurrentFocusUI != NewFocusUI)
		{
			CurrentFocusUI = NewFocusUI;
			const Vector2& MousePosition = GetInteractionComponent()->GetMousePosition();
			if (CurrentFocusUIFoundEvent)
				CurrentFocusUIFoundEvent(*this, CurrentFocusUI, MousePosition);
		}
	}
	void SetChangeFocusUIEvent(std::function<void(CUIToolPanel&, CUI*, const Vector2&)> InNewFocusUIFoundEvent)
	{
		CurrentFocusUIFoundEvent = InNewFocusUIFoundEvent;
	}
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

	std::function<void(CUIToolPanel&, CUI*, const Vector2&)> CurrentFocusUIFoundEvent;
};
