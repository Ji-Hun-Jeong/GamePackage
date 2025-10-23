#pragma once
#include <set>
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
		, CurrenFocusInteracter(nullptr)
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
				NextAddedInteracters.push(NewMouseInteracter);
			});
		return NewMouseInteracter;
	}

	void SetMousePositionInstance(CMousePositionInstance* InMousePositionInstance) { MousePositionInstance = InMousePositionInstance; }

	void SetMouseClick(EKeyType InKeyType, bool bInMouseClicked) { KeyType = InKeyType; bMouseClicked = bInMouseClicked; }
	void SetMouseRelease(EKeyType InKeyType, bool bInMouseReleased) { KeyType = InKeyType; bMouseReleased = bInMouseReleased; }
	void SetMouseMove(bool bInMouseMove) { bMouseMove = bInMouseMove; }

	bool IsMouseClicked() const { return bMouseClicked; }
	bool IsMouseReleased() const { return bMouseReleased; }
	bool IsMouseMove() const {return bMouseMove;}
	const Vector2& GetMousePosition() const { return MousePosition; }
	EKeyType GetKeyType() const { return KeyType; }

	void AddNextInteracter()
	{
		bMouseClicked = false;
		bMouseReleased = false;
		bMouseMove = false;
		while (NextAddedInteracters.empty() == false)
		{
			MouseInteracters.emplace_back(NextAddedInteracters.front());
			NextAddedInteracters.pop();
		}
		while (NextFrameEvents.empty() == false)
		{
			NextFrameEvents.front()();
			NextFrameEvents.pop();
		}
	}

	void FindCurrentInteracter()
	{
		if (MousePositionInstance == nullptr)
			return;

		for (auto& MouseInteracter : MouseInteracters)
			MouseInteracter->ClearState();

		MousePosition = MousePositionInstance->GetMousePosition();

		CMouseInteracter* NewFocusInteracter = nullptr;
		for (auto Iter = MouseInteracters.begin(); Iter != MouseInteracters.end();)
		{
			auto MouseInteracter = Iter->get();
			if (MouseInteracter->bDestroy)
			{
				if (MouseInteracter == CurrenFocusInteracter)
					CurrenFocusInteracter = nullptr;

				Iter = MouseInteracters.erase(Iter);
			}
			else
			{
				NewFocusInteracter = TryFindFocusInteracter(*MouseInteracter);
				if (NewFocusInteracter)
					FocusInteracterCandidates.insert(NewFocusInteracter);

				++Iter;
			}
		}

		CMouseInteracter* FinalNewFocusInteracter = nullptr;
		if (FocusInteracterCandidates.size())
		{
			FinalNewFocusInteracter = *FocusInteracterCandidates.begin();
			FocusInteracterCandidates.clear();
		}

		if (CurrenFocusInteracter != FinalNewFocusInteracter)
		{
			if (CurrenFocusInteracter)
				CurrenFocusInteracter->bMouseExit = true;

			if (FinalNewFocusInteracter)
				FinalNewFocusInteracter->bMouseEnter = true;
		}

		if (CurrenFocusInteracter)
			CurrenFocusInteracter->bMouseFocus = true;

		CurrenFocusInteracter = FinalNewFocusInteracter;
	}

private:
	CMouseInteracter* TryFindFocusInteracter(CMouseInteracter& InMouseInteracter)
	{
		bool bMouseOn = IsMouseOn(InMouseInteracter);
		if (bMouseOn == false)
			return nullptr;

		InMouseInteracter.bMouseOn = true;

		CMouseInteracter* NewFocusInteracter = &InMouseInteracter;
		for (auto& ChildInteracter : InMouseInteracter.ChildInteracters)
		{
			CMouseInteracter* NewInteracter = TryFindFocusInteracter(*ChildInteracter);
			if (NewInteracter)
				NewFocusInteracter = NewInteracter;
		}
		return NewFocusInteracter;
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
	Vector2 MousePosition;

	std::vector<std::unique_ptr<CMouseInteracter>> MouseInteracters;
	CMouseInteracter* CurrenFocusInteracter;
	std::set<CMouseInteracter*> FocusInteracterCandidates;

	std::queue<CMouseInteracter*> NextAddedInteracters;
	std::queue<std::function<void()>> NextFrameEvents;

	EKeyType KeyType;
	bool bMouseClicked;
	bool bMouseReleased;
	bool bMouseMove;

};

