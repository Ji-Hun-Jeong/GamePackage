#pragma once
#include "UI.h"

class CUIManager
{
public:
	CUIManager() = default;
	~CUIManager() = default;

private:
	class CFocusUICandidate
	{
	public:
		CFocusUICandidate() = default;
		~CFocusUICandidate() = default;

	public:
		void AddCandidate(CUI& InCandidate) { FocusUICandidates.push_back(&InCandidate); }
		bool IsEmpty() const { return FocusUICandidates.empty(); }
		void Merge(const CFocusUICandidate& InOther)
		{
			FocusUICandidates.insert(FocusUICandidates.end()
				, InOther.FocusUICandidates.begin(), InOther.FocusUICandidates.end());
		}
		CUI* GetOneFocusUI() const
		{
			CUI* FinalNewFocusUI = nullptr;
			if (FocusUICandidates.size())
				FinalNewFocusUI = *(--FocusUICandidates.end());

			return FinalNewFocusUI;
		}

	private:
		std::vector<CUI*> FocusUICandidates;
	};

public:
	const Vector2& GetMouseWorldPosition() const { return MouseWorldPosition; }
	void SetMouseWorldPosition(const class CMouseManager& InMouseManager, const class CCamera& InCamera);
	void PushUI(CUI& InUI)
	{
		Unwrapped(InUI);
		// CurrentFrameDetectUIs.push_back(&InUI);
	}
	void Unwrapped(CUI& InUI)
	{
		if (InUI.bInteraction == false)
			return;
		CurrentFrameDetectUIs.push_back(&InUI);
		for (auto ChildUI : InUI.ChildUIs)
			Unwrapped(*ChildUI);
	}
	void PushUIToEnd(CUI& InUI)
	{
		if (InUI.bInteraction == false)
			return;
		AddToEndUIs.push_back(&InUI);
	}
	void FindFocusUI()
	{
		CurrentFrameDetectUIs.insert(CurrentFrameDetectUIs.end(), AddToEndUIs.begin(), AddToEndUIs.end());

		std::vector<CUI*> FinalDetectUI;
		FinalDetectUI.reserve(CurrentFrameDetectUIs.size());

		for (auto UI : CurrentFrameDetectUIs)
		{
			if (IsMouseOn(*UI))
			{
				if (UI->MouseOnEvent)
					UI->MouseOnEvent();
				FinalDetectUI.push_back(UI);
			}
		}

		CUI* NewFocusUI = nullptr;
		if (FinalDetectUI.empty() == false)
		{
			std::stable_sort(FinalDetectUI.begin(), FinalDetectUI.end(), [](CUI* InA, CUI* InB)->bool {return InA->GetUILayer() < InB->GetUILayer(); });
			NewFocusUI = FinalDetectUI.back();
		}
		/*CFocusUICandidate FocusUICandidates;

		for (auto& UI : CurrentFrameDetectUIs)
		{
			CFocusUICandidate Candidates = TryFindFocusInteracters(*UI);
			if (Candidates.IsEmpty())
				continue;
			FocusUICandidates.Merge(Candidates);
		}

		CUI* NewFocusUI = FocusUICandidates.GetOneFocusUI();*/

		if (CurrentFocusUI != NewFocusUI)
		{
			if (CurrentFocusUI && CurrentFocusUI->MouseExitEvent)
				CurrentFocusUI->MouseExitEvent();

			if (NewFocusUI && NewFocusUI->MouseEnterEvent)
				NewFocusUI->MouseEnterEvent();
		}

		CurrentFocusUI = NewFocusUI;

		if (CurrentFocusUI && CurrentFocusUI->MouseFocusEvent)
			CurrentFocusUI->MouseFocusEvent();

		CurrentFrameDetectUIs.clear();
		AddToEndUIs.clear();
	}
private:
	CFocusUICandidate TryFindFocusInteracters(CUI& InUI)
	{
		bool bMouseOn = IsMouseOn(InUI);
		if (bMouseOn == false)
			return CFocusUICandidate();

		if (InUI.MouseOnEvent)
			InUI.MouseOnEvent();

		// 자식을 다 돌았을 때 최종적으로 아무 것도 포커싱이 안되있으면 자신이 포커싱 되어있는 상태
		CFocusUICandidate FinalCandidates;
		for (auto& ChildUI : InUI.ChildUIs)
		{
			CFocusUICandidate Candidates = TryFindFocusInteracters(*ChildUI);
			if (Candidates.IsEmpty())
				continue;
			FinalCandidates.Merge(Candidates);
		}
		if (FinalCandidates.IsEmpty())
			FinalCandidates.AddCandidate(InUI);

		return FinalCandidates;
	}
	bool IsMouseOn(const CUI& InUI)
	{
		Vector2 UISize;
		if (InUI.GetSpriteRenderComponent() && InUI.GetSpriteRenderComponent()->IsImageType())
			UISize = InUI.GetSpriteRenderComponent()->GetImageScale();
		else
			UISize = InUI.GetTransform()->GetScale2D();

		// 사각형의 중심 위치와 크기
		const Vector3& UIPosition = InUI.GetTransform()->GetFinalPosition();
		float centerX = UIPosition.x;
		float centerY = UIPosition.y;
		float halfWidth = UISize.x * 0.5f;
		float halfHeight = UISize.y * 0.5f;

		// 사각형의 경계
		float left = centerX - halfWidth;
		float right = centerX + halfWidth;
		float top = centerY + halfHeight;
		float bottom = centerY - halfHeight;

		// 마우스가 사각형 안에 있는지 확인
		return (MouseWorldPosition.x >= left && MouseWorldPosition.x <= right &&
			MouseWorldPosition.y >= bottom && MouseWorldPosition.y <= top);
	}

	bool IsInsideScreen(const CUI& InUI);
private:
	std::vector<CUI*> CurrentFrameDetectUIs;
	std::vector<CUI*> AddToEndUIs;
	CUI* CurrentFocusUI;

	Vector2 MouseWorldPosition;
	Vector3 CameraPosition;

};

