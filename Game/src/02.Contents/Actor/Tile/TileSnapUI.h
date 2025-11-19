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
			CenterMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Left:
			LeftMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Right:
			RightMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Top:
			TopMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::Bottom:
			BottomMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::LeftTop:
			LeftTopMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::LeftBottom:
			LeftBottomMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::RightTop:
			RightTopMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		case ETilePositionType::RightBottom:
			RightBottomMoveUI->GetInteractionComponent()->SetMouseFocusEvent(InEvent);
			break;
		default:
			assert(0);
			break;
		}
	}

	void AppearUI()
	{
		LeftMoveUI->GetSpriteRenderComponent()->SetRender(true);
		RightMoveUI->GetSpriteRenderComponent()->SetRender(true);
		TopMoveUI->GetSpriteRenderComponent()->SetRender(true);
		BottomMoveUI->GetSpriteRenderComponent()->SetRender(true);
		CenterMoveUI->GetSpriteRenderComponent()->SetRender(true);
		LeftTopMoveUI->GetSpriteRenderComponent()->SetRender(true);
		LeftBottomMoveUI->GetSpriteRenderComponent()->SetRender(true);
		RightTopMoveUI->GetSpriteRenderComponent()->SetRender(true);
		RightBottomMoveUI->GetSpriteRenderComponent()->SetRender(true);

		LeftMoveUI->GetInteractionComponent()->SetInteraction(true);
		RightMoveUI->GetInteractionComponent()->SetInteraction(true);
		TopMoveUI->GetInteractionComponent()->SetInteraction(true);
		BottomMoveUI->GetInteractionComponent()->SetInteraction(true);
		CenterMoveUI->GetInteractionComponent()->SetInteraction(true);
		LeftTopMoveUI->GetInteractionComponent()->SetInteraction(true);
		LeftBottomMoveUI->GetInteractionComponent()->SetInteraction(true);
		RightTopMoveUI->GetInteractionComponent()->SetInteraction(true);
		RightBottomMoveUI->GetInteractionComponent()->SetInteraction(true);

		LeftMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		RightMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		TopMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		BottomMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		CenterMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		LeftTopMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		LeftBottomMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		RightTopMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
		RightBottomMoveUI->GetInteractionComponent()->SetEndInteractionCheck(true);
	}

	void DisappearUI()
	{
		LeftMoveUI->GetSpriteRenderComponent()->SetRender(false);
		RightMoveUI->GetSpriteRenderComponent()->SetRender(false);
		TopMoveUI->GetSpriteRenderComponent()->SetRender(false);
		BottomMoveUI->GetSpriteRenderComponent()->SetRender(false);
		CenterMoveUI->GetSpriteRenderComponent()->SetRender(false);
		LeftTopMoveUI->GetSpriteRenderComponent()->SetRender(false);
		LeftBottomMoveUI->GetSpriteRenderComponent()->SetRender(false);
		RightTopMoveUI->GetSpriteRenderComponent()->SetRender(false);
		RightBottomMoveUI->GetSpriteRenderComponent()->SetRender(false);

		LeftMoveUI->GetInteractionComponent()->SetInteraction(false);
		RightMoveUI->GetInteractionComponent()->SetInteraction(false);
		TopMoveUI->GetInteractionComponent()->SetInteraction(false);
		BottomMoveUI->GetInteractionComponent()->SetInteraction(false);
		CenterMoveUI->GetInteractionComponent()->SetInteraction(false);
		LeftTopMoveUI->GetInteractionComponent()->SetInteraction(false);
		LeftBottomMoveUI->GetInteractionComponent()->SetInteraction(false);
		RightTopMoveUI->GetInteractionComponent()->SetInteraction(false);
		RightBottomMoveUI->GetInteractionComponent()->SetInteraction(false);
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

};

