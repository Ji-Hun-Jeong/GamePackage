#pragma once
#include "MouseInteracter.h"

class CMousePosition
{
public:
	CMousePosition() = default;
	~CMousePosition() = default;

public:
	void SetMousePosition(const Vector2& InMousePosition) { MousePosition = InMousePosition; }
	const Vector2& GetMousePosition() const { return MousePosition; }

private:
	Vector2 MousePosition;

};

class CMouseManager
{
	friend class CSetMouseInteracter;
public:
	CMouseManager()
		: MousePosition(nullptr)
		, CurrentInteracter(nullptr)
		, bMouseClicked(false)
		, bMouseReleased(false)
	{}
	~CMouseManager() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	CMouseInteracter* NewMouseInteracter()
	{
		CMouseInteracter* NewMouseInteracter = new CMouseInteracter;
		MouseInteracters.emplace_back(NewMouseInteracter);

		return NewMouseInteracter;
	}
	void SetMouseClick(bool bInMouseClicked) { bMouseClicked = bInMouseClicked; }
	void SetMouseRelease(bool bInMouseReleased) { bMouseReleased = bInMouseReleased; }

	void FindCurrentInteracter()
	{
		if (MousePosition == nullptr)
			return;

		CMouseInteracter* NewInteracter = nullptr;
		for (auto Iter = MouseInteracters.begin(); Iter != MouseInteracters.end();)
		{
			auto MouseInteracter = Iter->get();
			if (MouseInteracter->bDestroy)
			{
				if (MouseInteracter == CurrentInteracter)
				{
					bMouseReleased ? MouseInteracter->ActivateMouseReleaseEvent() : MouseInteracter->ActivateMouseExitEvent();
					CurrentInteracter = nullptr;
				}
				Iter = MouseInteracters.erase(Iter);
			}
			else
			{
				NewInteracter = TryFindOnInteracter(*MouseInteracter);
				if (NewInteracter)
					break;
				++Iter;
			}
		}

		if (CurrentInteracter != NewInteracter)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseExitEvent();

			if (NewInteracter)
				NewInteracter->ActivateMouseEnterEvent();
		}

		CurrentInteracter = NewInteracter;

		if (bMouseClicked)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseClickEvent();
			bMouseClicked = false;
		}
		if (bMouseReleased)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseReleaseEvent();
			bMouseReleased = false;
		}
	}

private:
	CMouseInteracter* TryFindOnInteracter(CMouseInteracter& InMouseInteracter)
	{
		bool bMouseOn = IsMouseOn(InMouseInteracter);
		if (bMouseOn == false)
			return nullptr;

		CMouseInteracter* NewInteracter = &InMouseInteracter;
		for (auto& ChildInteracter : InMouseInteracter.ChildInteracters)
		{
			NewInteracter = TryFindOnInteracter(*ChildInteracter);
			if (NewInteracter)
				break;
		}
		return NewInteracter;
	}
	bool IsMouseOn(const CMouseInteracter& InMouseInteracter)
	{
		if (InMouseInteracter.Size.x == 0.0f && InMouseInteracter.Size.y == 0.0f)
		{
			std::cout << "MouseInteracterSize Is Zero\n";
			return false;
		}

		// 사각형의 중심 위치와 크기
		float centerX = InMouseInteracter.Position.x;
		float centerY = InMouseInteracter.Position.y;
		float halfWidth = InMouseInteracter.Size.x * 0.5f;
		float halfHeight = InMouseInteracter.Size.y * 0.5f;

		// 사각형의 경계
		float left = centerX - halfWidth;
		float right = centerX + halfWidth;
		float top = centerY + halfHeight;
		float bottom = centerY - halfHeight;

		// 마우스가 사각형 안에 있는지 확인
		return (MousePosition->GetMousePosition().x >= left && MousePosition->GetMousePosition().x <= right &&
			MousePosition->GetMousePosition().y >= bottom && MousePosition->GetMousePosition().y <= top);
	}
	
private:
	std::unique_ptr<CMousePosition> MousePosition;

	std::vector<std::unique_ptr<CMouseInteracter>> MouseInteracters;
	CMouseInteracter* CurrentInteracter;

	bool bMouseClicked;
	bool bMouseReleased;

};

