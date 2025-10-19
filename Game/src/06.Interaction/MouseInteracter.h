#pragma once

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
		/*MouseEnterEvent = nullptr;
		MouseExitEvent = nullptr;
		MouseClickEvent = nullptr;
		MouseReleaseEvent = nullptr;*/
	}
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void SetSize(const Vector2& InSize) { Size = InSize; }
	void SetMouseEnterEvent(std::function<void()> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void()> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseClickEvent(std::function<void()> InMouseClickEvent) { MouseClickEvent = InMouseClickEvent; }
	void SetMouseReleaseEvent(std::function<void()> InMouseReleaseEvent) { MouseReleaseEvent = InMouseReleaseEvent; }

private:
	void ActivateMouseEnterEvent()
	{
		if (MouseEnterEvent)
			MouseEnterEvent();
	}
	void ActivateMouseExitEvent()
	{
		if (MouseExitEvent)
			MouseExitEvent();
	}
	void ActivateMouseClickEvent()
	{
		if (MouseClickEvent)
			MouseClickEvent();
	}
	void ActivateMouseReleaseEvent()
	{
		if (MouseReleaseEvent)
			MouseReleaseEvent();
	}

private:
	Vector2 Position;
	Vector2 Size;

	std::function<void()> MouseEnterEvent;
	std::function<void()> MouseExitEvent;
	std::function<void()> MouseClickEvent;
	std::function<void()> MouseReleaseEvent;

	std::vector<CMouseInteracter*> ChildInteracters;

	bool bDestroy;

};

