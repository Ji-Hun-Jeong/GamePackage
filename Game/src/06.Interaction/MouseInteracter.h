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

	bool IsMouseEnter() const { return bMouseEnter; }
	bool IsMouseExit() const { return bMouseExit; }
	bool IsMouseOn() const { return bMouseOn; }
	bool IsMouseFocus() const { return bMouseFocus; }

private:
	void ClearState()
	{
		bMouseEnter = false;
		bMouseExit = false;
		bMouseOn = false;
		bMouseFocus = false;
	}

private:
	Vector2 Position;
	Vector2 Size;

	bool bMouseEnter = false;
	bool bMouseExit = false;
	bool bMouseOn = false;
	bool bMouseFocus = false;

	std::vector<CMouseInteracter*> ChildInteracters;

	inline static int32_t MouseX;
	inline static int32_t MouseY;

};

