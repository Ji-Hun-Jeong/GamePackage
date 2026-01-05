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
		CUI* GetOneFocusUI()
		{
			CUI* FinalNewFocusUI = nullptr;
			if (FocusUICandidates.empty() == false)
			{
				std::stable_sort(FocusUICandidates.begin(), FocusUICandidates.end(), [](CUI* InA, CUI* InB)->bool {return InA->GetFinalUILayer() < InB->GetFinalUILayer(); });
				FinalNewFocusUI = FocusUICandidates.back();
			}

			return FinalNewFocusUI;
		}

	private:
		std::vector<CUI*> FocusUICandidates;
	};

public:
	const Vector2& GetMouseWorldPosition() const { return MouseWorldPosition; }
	void SetMouseWorldPosition(const class CMouseManager& InMouseManager, const class CCamera& InCamera);
	void AddUI(CUI& InUI)
	{
		CurrentFrameDetectUIs.push_back(&InUI);
	}
	void AddUIToEnd(CUI& InUI)
	{
		AddToEndUIs.push_back(&InUI);
	}
	void FindFocusUI()
	{
		std::vector<CUI*> FinalDetectUI = CurrentFrameDetectUIs;
		FinalDetectUI.insert(FinalDetectUI.end(), AddToEndUIs.begin(), AddToEndUIs.end());

		for (size_t i = 0; i < FinalDetectUI.size(); ++i)
		{
			CUI* UI = FinalDetectUI[i];
			GetFinalLayer(*UI);
		}

		CFocusUICandidate FocusUICandidates;

		for (auto& UI : FinalDetectUI)
		{
			CFocusUICandidate Candidates = TryFindFocusInteracters(*UI);
			if (Candidates.IsEmpty())
				continue;
			FocusUICandidates.Merge(Candidates);
		}

		CUI* NewFocusUI = FocusUICandidates.GetOneFocusUI();

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
	}
private:
	void GetFinalLayer(CUI& InOwnerUI)
	{
		for (auto ChildUI : InOwnerUI.ChildUIs)
		{
			ChildUI->SetFinalUILayer(InOwnerUI.UILayer + ChildUI->UILayer + 1);
			GetFinalLayer(*ChildUI);
		}
	}

private:
	CFocusUICandidate TryFindFocusInteracters(CUI& InUI)
	{
		if (InUI.bInteraction == false)
			return CFocusUICandidate();

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
		const Vector3& UIPosition = InUI.GetTransform()->GetPosition();
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

