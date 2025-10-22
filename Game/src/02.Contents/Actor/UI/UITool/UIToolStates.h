#pragma once
#include <stack>
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/UI.h"

class CUIToolState : public CActor
{
	GENERATE_OBJECT()
public:
	CUIToolState() = default;
	~CUIToolState() = default;

	std::function<void(const Vector2&)> MouseEnterEvent;
	std::function<void(const Vector2&)> MouseExitEvent;
	std::function<void(const Vector2&)> MouseMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseReleaseEvent;

};

class CUIToolInputState : public CUIToolState
{
	GENERATE_OBJECT()
public:
	void PlaceUIOnToolPanel(EKeyType InKeyType, const std::wstring& InUIImage, const Vector2& InMouseWorldPosition, class CUIToolPanel& InUIToolPanel);

private:
	CUI* DoPlaceUI(const Vector3& InPosition, const std::wstring& InImagePath, class CUIToolPanel& InUIToolPanel);
	void UndoPlaceUI();

private:
	std::stack<CUI*> UIs;

};