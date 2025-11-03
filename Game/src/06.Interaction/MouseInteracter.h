#pragma once
#include <Core/public/InputManager.h>
#include "MouseManager.h"

class CMouseInteracter
{
	friend class CMouseInteractionManager;
public:
	CMouseInteracter()
		: Size(0.0f)
	{}
	~CMouseInteracter() = default;

public:
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void SetSize(const Vector2& InSize) { Size = InSize; }

	void SetMouseEnterEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseOnEvent(std::function<void(const Vector2&)> InMouseOnEvent) { MouseOnEvent = InMouseOnEvent; }
	void SetMouseFocusEvent(std::function<void(const Vector2&)> InMouseFocusEvent) { MouseFocusEvent = InMouseFocusEvent; }

	void AttachChildInteracter(CMouseInteracter* InChildInteracter) { ChildInteracters.push_back(InChildInteracter); }
	void DetachChildInteracter(CMouseInteracter* InChildInteracter)
	{
		for (auto Iter = ChildInteracters.begin(); Iter != ChildInteracters.end(); ++Iter)
		{
			if (*Iter == InChildInteracter)
			{
				ChildInteracters.erase(Iter);
				break;
			}
		}
	}
	static int32_t GetCurrentMouseX() { return MouseX; }
	static int32_t GetCurrentMouseY() { return MouseY; }

private:
	void ActivateMouseEnterEvent(const Vector2& InMousePosition) const { if (MouseEnterEvent) MouseEnterEvent(InMousePosition); }
	void ActivateMouseExitEvent(const Vector2& InMousePosition) const { if (MouseExitEvent) MouseExitEvent(InMousePosition); }
	void ActivateMouseOnEvent(const Vector2& InMousePosition) const { if (MouseOnEvent) MouseOnEvent(InMousePosition); }
	void ActivateMouseFocusEvent(const Vector2& InMousePosition) const { if (MouseFocusEvent) MouseFocusEvent(InMousePosition); }

private:
	Vector2 Position;
	Vector2 Size;

	std::function<void(const Vector2&)> MouseEnterEvent = nullptr;
	std::function<void(const Vector2&)> MouseExitEvent = nullptr;
	std::function<void(const Vector2&)> MouseOnEvent = nullptr;
	std::function<void(const Vector2&)> MouseFocusEvent = nullptr;

	std::vector<CMouseInteracter*> ChildInteracters;

	inline static int32_t MouseX;
	inline static int32_t MouseY;

};

