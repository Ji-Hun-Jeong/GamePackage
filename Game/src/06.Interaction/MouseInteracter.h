#pragma once
#include <Core/public/InputManager.h>

class CMouseInteracter
{
	friend class CMouseManager;
public:
	CMouseInteracter()
		: Size(0.0f)
		, MouseEnterEvent(nullptr)
		, MouseClickEvent(nullptr)
		, bDestroy(false)
	{}
	~CMouseInteracter() = default;

public:
	void Destroy()
	{
		bDestroy = true;
	}
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void SetSize(const Vector2& InSize) { Size = InSize; }
	void SetMouseEnterEvent(std::function<void(const Vector2&)> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void(const Vector2&)> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseClickEvent(std::function<void(EKeyType InKeyType, const Vector2&)> InMouseClickEvent) { MouseClickEvent = InMouseClickEvent; }
	void SetMouseReleaseEvent(std::function<void(EKeyType InKeyType, const Vector2&)> InMouseReleaseEvent) { MouseReleaseEvent = InMouseReleaseEvent; }
	void SetMouseMoveEvent(std::function<void(const Vector2&)> InMouseMoveEvent) { MouseMoveEvent = InMouseMoveEvent; }
	void AttachChildInteracter(CMouseInteracter* InChildInteracter) { ChildInteracters.push_back(InChildInteracter); }
	void DetachChildInteracter(CMouseInteracter* InChildInteracter)
	{
		for (auto Iter = ChildInteracters.begin(); Iter != ChildInteracters.end(); ++Iter)
		{
			if ((*Iter) == InChildInteracter)
			{
				ChildInteracters.erase(Iter);
				break;
			}
		}
	}

private:
	void ActivateMouseEnterEvent(const Vector2& InMousePosition)
	{
		if (MouseEnterEvent)
			MouseEnterEvent(InMousePosition);
	}
	void ActivateMouseExitEvent(const Vector2& InMousePosition)
	{
		if (MouseExitEvent)
			MouseExitEvent(InMousePosition);
	}
	void ActivateMouseClickEvent(EKeyType InKeyType, const Vector2& InMousePosition)
	{
		if (MouseClickEvent)
			MouseClickEvent(InKeyType, InMousePosition);
	}
	void ActivateMouseReleaseEvent(EKeyType InKeyType, const Vector2& InMousePosition)
	{
		if (MouseReleaseEvent)
			MouseReleaseEvent(InKeyType, InMousePosition);
	}
	void ActivateMouseMoveEvent(const Vector2& InMousePosition)
	{
		if (MouseMoveEvent)
			MouseMoveEvent(InMousePosition);
	}

private:
	Vector2 Position;
	Vector2 Size;

	std::function<void(const Vector2&)> MouseEnterEvent;
	std::function<void(const Vector2&)> MouseExitEvent;
	std::function<void(const Vector2&)> MouseMoveEvent;
	std::function<void(EKeyType, const Vector2&)> MouseClickEvent;
	std::function<void(EKeyType, const Vector2&)> MouseReleaseEvent;

	std::vector<CMouseInteracter*> ChildInteracters;

	bool bDestroy;

};

