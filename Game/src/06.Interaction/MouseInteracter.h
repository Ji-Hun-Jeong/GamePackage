#pragma once
#include <Core/public/InputManager.h>

class CMouseInteracter
{
	friend class CMouseManager;
public:
	CMouseInteracter()
		: Size(0.0f)
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

	bool IsMouseEnter() const { return bMouseEnter; }
	bool IsMouseOn() const { return bMouseOn; }
	bool IsMouseFocus() const { return bMouseFocus; }
	bool IsMouseExit() const { return bMouseExit; }

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
	bool bMouseOn = false;	// 올라가 있는 상태
	bool bMouseFocus = false;	// 가장 자식, 포커싱 되있는 상태
	bool bMouseExit = false;

	std::vector<CMouseInteracter*> ChildInteracters;

	bool bDestroy;

};

