#pragma once
#include <stack>
#include "01.Base/Object/UI.h"
#include "UIToolPanel.h"

class IUIToolState : public CActor
{
	friend class CUIToolPanelManager;
public:
	IUIToolState() : UIToolPanel(nullptr) {}
	virtual ~IUIToolState() = 0 {}

public:
	CUIToolPanel* GetUIToolPanel() const { return UIToolPanel; }

	virtual void EnterState() {}
	virtual void ExitState() {}
	virtual void MouseEnter(const Vector2& InMousePosition) {}
	virtual void MouseExit(const Vector2& InMousePosition) {}
	virtual void MouseMove(const Vector2& InMousePosition) {}
	virtual void MouseClick(EKeyType InKeyType, const Vector2& InMousePosition) {}
	virtual void MouseRelease(EKeyType InKeyType, const Vector2& InMousePosition) {}

protected:
	virtual void SetUIToolPanel(CUIToolPanel* InUIToolPanel) 
	{ 
		UIToolPanel = InUIToolPanel; 
	}

private:
	CUIToolPanel* UIToolPanel;

};

class CUIToolInputState : public IUIToolState
{
	GENERATE_OBJECT()
public:
	CUIToolInputState() 
		: LoadDialogUI(nullptr)
		, MousePointer(nullptr) 
		, WindowIOManager(nullptr)
	{}
	~CUIToolInputState() = default;

public:
	void EnterState() override;
	void ExitState() override;
	void MouseRelease(EKeyType InKeyType, const Vector2& InMousePosition) override;
	void InitalizeInputState(CUI& InLoadDialogUI, class CWindowIOManager& InWindowIOManager, class CMousePointer& InMousePointer);

private:
	CUI* LoadDialogUI;
	class CWindowIOManager* WindowIOManager;
	class CMousePointer* MousePointer;

	std::stack<CUI*> UIStack;

};

class CUIToolMoveState : public IUIToolState
{
	GENERATE_OBJECT()
public:
	CUIToolMoveState() {}
	~CUIToolMoveState() {}

public:
	void SetUIToolPanel(CUIToolPanel* InUIToolPanel) override
	{
		IUIToolState::SetUIToolPanel(InUIToolPanel);
		Index = InUIToolPanel->AddCreateChildUIEvent([this](CUI& InNewUI)->void
			{
				SetCurrentFocusUI(InNewUI);
			});
	}
	void SetCurrentFocusUI(CUI& InUI)
	{
		// Todo: 포커싱 상태가 아닐 때도 세팅해주는거 열받는데
		InUI.GetInteractionComponent()->SetMouseClickEvent([this, &InUI](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				CurrentFocusUI = &InUI;
			});
		InUI.GetInteractionComponent()->SetMouseReleaseEvent([this](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				CurrentFocusUI = nullptr;
			});
	}
	void MouseMove(const Vector2& InMousePosition) override
	{
		static int a = 0;
		//std::cout << "Move" << a++ << std::endl;
		if (CurrentFocusUI == nullptr)
			return;

		Vector3 NewPosition = Vector3(InMousePosition.x, InMousePosition.y, CurrentFocusUI->GetTransform()->GetPosition().z);
		CurrentFocusUI->GetTransform()->SetPosition(NewPosition);
	}

private:
	CUI* CurrentFocusUI = nullptr;
	size_t Index = 0;

};