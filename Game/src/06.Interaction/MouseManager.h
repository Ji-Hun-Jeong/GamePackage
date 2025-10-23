#pragma once
#include "MouseInteracter.h"

class CMousePositionInstance
{
public:
	CMousePositionInstance() = default;
	~CMousePositionInstance() = default;

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
		: MousePositionInstance(nullptr)
		, CurrentInteracter(nullptr)
		, bMouseClicked(false)
		, bMouseReleased(false)
		, bMouseMove(false)
		, KeyType(EKeyType::End)
	{}
	~CMouseManager() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	CMouseInteracter* NewMouseInteracter(CMouseInteracter* InOwnerMouseInteracter)
	{
		CMouseInteracter* NewMouseInteracter = new CMouseInteracter;
		NextFrameEvents.push([this, NewMouseInteracter, InOwnerMouseInteracter]()->void
			{
				if (InOwnerMouseInteracter)
					InOwnerMouseInteracter->AttachChildInteracter(NewMouseInteracter);
				NextAddedInteracters.emplace(NewMouseInteracter);
			});
		return NewMouseInteracter;
	}
	void SetMousePositionInstance(CMousePositionInstance* InMousePositionInstance) { MousePositionInstance = InMousePositionInstance; }
	void SetMouseClick(EKeyType InKeyType, bool bInMouseClicked) { KeyType = InKeyType; bMouseClicked = bInMouseClicked; }
	void SetMouseRelease(EKeyType InKeyType, bool bInMouseReleased) { KeyType = InKeyType; bMouseReleased = bInMouseReleased; }
	void SetMouseMove(bool bInMouseMove) { bMouseMove = bInMouseMove; }

	void FindCurrentInteracter()
	{
		if (MousePositionInstance == nullptr)
			return;

		const Vector2& FinalMousePosition = MousePositionInstance->GetMousePosition();
		CMouseInteracter* NewInteracter = nullptr;
		for (auto Iter = MouseInteracters.begin(); Iter != MouseInteracters.end();)
		{
			auto MouseInteracter = Iter->get();
			if (MouseInteracter->bDestroy)
			{
				if (MouseInteracter == CurrentInteracter)
				{
					bMouseReleased || bMouseClicked ? MouseInteracter->ActivateMouseReleaseEvent(KeyType, FinalMousePosition)
						: MouseInteracter->ActivateMouseExitEvent(FinalMousePosition);
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
				CurrentInteracter->ActivateMouseExitEvent(FinalMousePosition);

			if (NewInteracter)
				NewInteracter->ActivateMouseEnterEvent(FinalMousePosition);
		}

		CurrentInteracter = NewInteracter;

		if (bMouseClicked)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseClickEvent(KeyType, FinalMousePosition);
			bMouseClicked = false;
		}
		if (bMouseReleased)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseReleaseEvent(KeyType, FinalMousePosition);
			bMouseReleased = false;
		}
		if (bMouseMove)
		{
			if (CurrentInteracter)
				CurrentInteracter->ActivateMouseMoveEvent(FinalMousePosition);
			bMouseMove = false;
		}
	}
	void AddNextInteracter()
	{
		while (NextAddedInteracters.empty() == false)
		{
			MouseInteracters.push_back(std::move(NextAddedInteracters.front()));
			NextAddedInteracters.pop();
		}
		while (NextFrameEvents.empty() == false)
		{
			NextFrameEvents.front()();
			NextFrameEvents.pop();
		}
	}

private:
	CMouseInteracter* TryFindOnInteracter(CMouseInteracter& InMouseInteracter)
	{
		bool bMouseOn = IsMouseOn(InMouseInteracter);
		if (bMouseOn == false)
			return nullptr;

		for (auto& ChildInteracter : InMouseInteracter.ChildInteracters)
		{
			CMouseInteracter* NewInteracter = TryFindOnInteracter(*ChildInteracter);
			if (NewInteracter)
				return NewInteracter;
		}
		return &InMouseInteracter;
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
		return (MousePositionInstance->GetMousePosition().x >= left && MousePositionInstance->GetMousePosition().x <= right &&
			MousePositionInstance->GetMousePosition().y >= bottom && MousePositionInstance->GetMousePosition().y <= top);
	}

private:
	CMousePositionInstance* MousePositionInstance;

	std::vector<std::unique_ptr<CMouseInteracter>> MouseInteracters;
	CMouseInteracter* CurrentInteracter;
	std::queue<std::unique_ptr<CMouseInteracter>> NextAddedInteracters;
	std::queue<std::function<void()>> NextFrameEvents;

	EKeyType KeyType;
	bool bMouseClicked;
	bool bMouseReleased;
	bool bMouseMove;

};

