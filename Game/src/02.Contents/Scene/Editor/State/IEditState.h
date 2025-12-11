#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/UI.h"
#include "04.Renderer/ImGuiManager.h"

class IEditState : public CActor
{
public:
	IEditState() = default;
	virtual ~IEditState() = default;

public:
	virtual void EnterEditState(CUI& InMainPanel) = 0;
	virtual void OnEditState(CUI& InMainPanel) = 0;
	virtual void ExitEditState(CUI& InMainPanel) = 0;
	virtual void ToImGUI() {}

	void SetMouseWorldPosition(const Vector2& InMouseWorldPosition) { MouseWorldPosition = InMouseWorldPosition; }
	const Vector2& GetMouseWorldPosition() const { return MouseWorldPosition; }

private:
	Vector2 MouseWorldPosition;
};

