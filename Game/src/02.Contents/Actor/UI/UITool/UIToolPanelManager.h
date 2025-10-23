#pragma once
#include "01.Base/Object/Actor.h"
#include "02.Contents/Actor/Base/MousePointer.h"
#include "UIToolStates.h"
#include "UIToolPanel.h"

class CUIToolPanelManager : public CActor
{
	GENERATE_OBJECT()
public:
	CUIToolPanelManager() = default;
	~CUIToolPanelManager() = default;

public:
	void Initalize() override;
	void Destroy() override
	{
		CActor::Destroy();
		for (auto& UIToolState : UIToolStates)
			UIToolState->Destroy();
	}
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
	}
	void InitUIToolPanelManager(CUIToolPanel& InUIToolPanel, CMousePointer& InMousePointer)
	{
		UIToolPanel = &InUIToolPanel;
		MousePointer = &InMousePointer; 
		assert(UIToolPanel);
		assert(MousePointer);
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

	CMousePointer* MousePointer;

};

