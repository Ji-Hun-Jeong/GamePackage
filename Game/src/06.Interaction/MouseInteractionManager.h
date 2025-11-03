#pragma once
#include <set>
#include "MouseInteracter.h"
#include "MouseManager.h"

class CFoucsInteracterCandidate
{
public:
	CFoucsInteracterCandidate() = default;
	~CFoucsInteracterCandidate() = default;

public:
	void AddCandidate(CMouseInteracter& InCandidate) { FocusInteracterCandidates.push_back(&InCandidate); }
	bool IsEmpty() const { return FocusInteracterCandidates.empty(); }
	void Merge(const CFoucsInteracterCandidate& InOther)
	{
		FocusInteracterCandidates.insert(FocusInteracterCandidates.begin()
			, InOther.FocusInteracterCandidates.begin(), InOther.FocusInteracterCandidates.end());
	}
	CMouseInteracter* GetOneFocusInteracter() const
	{
		CMouseInteracter* FinalNewFocusInteracter = nullptr;
		if (FocusInteracterCandidates.size())
			FinalNewFocusInteracter = *(--FocusInteracterCandidates.end());

		return FinalNewFocusInteracter;
	}

private:
	std::vector<CMouseInteracter*> FocusInteracterCandidates;

};
class CMouseInteractionManager
{
public:
	CMouseInteractionManager()
		: CurrentFocusInteracter(nullptr)
	{}
	~CMouseInteractionManager() = default;

public:
	void FindFocusInteracter(std::vector<CMouseInteracter*>&& InMouseInteracters, const Vector2& InMousePosition)
	{
		CurrentFrameDetectInteracters = std::move(InMouseInteracters);

		CMouseInteracter::MouseX = int32_t(InMousePosition.x);
		CMouseInteracter::MouseY = int32_t(InMousePosition.y);

		CFoucsInteracterCandidate FocusInteracterCandidates;

		for (auto& MouseInteracter : CurrentFrameDetectInteracters)
		{
			CFoucsInteracterCandidate Candidates= TryFindFocusInteracters(*MouseInteracter, InMousePosition);
			if (Candidates.IsEmpty())
				continue;
			FocusInteracterCandidates.Merge(Candidates);
		}

		CMouseInteracter* NewFocusInteracter = FocusInteracterCandidates.GetOneFocusInteracter();

		if (CurrentFocusInteracter != NewFocusInteracter)
		{
			if (CurrentFocusInteracter)
				CurrentFocusInteracter->bMouseExit = true;

			if (NewFocusInteracter)
				NewFocusInteracter->bMouseEnter = true;
		}

		if (CurrentFocusInteracter)
			CurrentFocusInteracter->bMouseFocus = true;

		CurrentFocusInteracter = NewFocusInteracter;
	}
	void ClearState()
	{
		for (auto& CurrentFrameDetectInteracter : CurrentFrameDetectInteracters)
			CurrentFrameDetectInteracter->ClearState();
	}

private:
	CFoucsInteracterCandidate TryFindFocusInteracters(CMouseInteracter& InMouseInteracter, const Vector2& InMousePosition)
	{
		bool bMouseOn = IsMouseOn(InMouseInteracter, InMousePosition);
		if (bMouseOn == false)
			return CFoucsInteracterCandidate();

		InMouseInteracter.bMouseOn = true;

		// 자식을 다 돌았을 때 최종적으로 아무 것도 포커싱이 안되있으면 자신이 포커싱 되어있는 상태
		CFoucsInteracterCandidate FinalCandidates;
		for (auto& ChildInteracter : InMouseInteracter.ChildInteracters)
		{
			CFoucsInteracterCandidate Candidates = TryFindFocusInteracters(*ChildInteracter, InMousePosition);
			if (Candidates.IsEmpty())
				continue;
			FinalCandidates.Merge(Candidates);
		}
		if (FinalCandidates.IsEmpty())
			FinalCandidates.AddCandidate(InMouseInteracter);

		return FinalCandidates;
	}

	bool IsMouseOn(CMouseInteracter& InMouseInteracter, const Vector2& InMousePosition)
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
		return (InMousePosition.x >= left && InMousePosition.x <= right &&
			InMousePosition.y >= bottom && InMousePosition.y <= top);
	}

private:
	std::vector<CMouseInteracter*> CurrentFrameDetectInteracters;
	CMouseInteracter* CurrentFocusInteracter;

};

