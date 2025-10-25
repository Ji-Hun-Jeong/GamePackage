#pragma once
#include "01.Base/Object/Actor.h"
#include "UIToolStates.h"
#include "UIToolPanel.h"

class CUIToolPanelManager : public CActor
{
	GENERATE_OBJECT(CUIToolPanelManager)
public:
	CUIToolPanelManager() = default;
	~CUIToolPanelManager() = default;

public:
	void Initalize() override;
	void Destroy() override
	{
		CActor::Destroy();
		if (CurrentUIToolState)
			CurrentUIToolState->ExitState(*UIToolPanel);
		UIToolStates.clear();
	}
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
	}
	void InitUIToolPanelManager(CUIToolPanel& InUIToolPanel)
	{
		UIToolPanel = &InUIToolPanel;
		assert(UIToolPanel);
	}
	void AddUIToolState(std::unique_ptr<IUIToolState> InUIToolState)
	{
		assert(InUIToolState);
		UIToolStates.push_back(std::move(InUIToolState));
	}
	void SetCurrentUIToolState(IUIToolState* InUIToolState)
	{
		assert(InUIToolState);
		if (CurrentUIToolState)
			CurrentUIToolState->ExitState(*UIToolPanel);
		CurrentUIToolState = InUIToolState;
		CurrentUIToolState->EnterState(*UIToolPanel);
	}

private:
	CUIToolPanel* UIToolPanel;

	std::vector<std::unique_ptr<IUIToolState>> UIToolStates;
	IUIToolState* CurrentUIToolState;

};

