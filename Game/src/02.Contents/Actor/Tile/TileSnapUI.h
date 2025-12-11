#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/UI.h"
#include "Tile.h"

class CTileSnapUI : public CActor
{
	GENERATE_OBJECT(CTileSnapUI)
public:
	CTileSnapUI();
	~CTileSnapUI() = default;

public:
	void SetUIEvent(ETilePositionType InTilePositionType, std::function<void()> InEvent)
	{
		switch (InTilePositionType)
		{
		case ETilePositionType::Center:
			CenterMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Left:
			LeftMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Right:
			RightMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Top:
			TopMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Bottom:
			BottomMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::LeftTop:
			LeftTopMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::LeftBottom:
			LeftBottomMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::RightTop:
			RightTopMoveUI->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::RightBottom:
			RightBottomMoveUI->SetMouseFocusEvent(InEvent);
			break;
		default:
			assert(0);
			break;
		}
	}
	void Activate(bool bInActivate) override
	{
		CActor::Activate(bInActivate);

		for (auto UI : UIs)
			UI->Activate(bInActivate);
	}
	void AttachToUI(CUI& InOwnerUI)
	{
		OwnerUI = &InOwnerUI;
		for (auto UI : UIs)
			InOwnerUI.AttachChildUI(*UI);
	}
	void DetachToUI()
	{
		if (OwnerUI == nullptr)
			return;

		for (auto UI : UIs)
			OwnerUI->DetachChildUI(*UI);
		OwnerUI = nullptr;
	}

private:
	void InitalizeMoveUI(CUI* InMoveUI);

private:
	CUI* LeftMoveUI = nullptr;
	CUI* RightMoveUI = nullptr;
	CUI* TopMoveUI = nullptr;
	CUI* BottomMoveUI = nullptr;
	CUI* CenterMoveUI = nullptr;
	CUI* LeftTopMoveUI = nullptr;
	CUI* LeftBottomMoveUI = nullptr;
	CUI* RightTopMoveUI = nullptr;
	CUI* RightBottomMoveUI = nullptr;

	std::vector<CUI*> UIs;

	CUI* OwnerUI = nullptr;
};

