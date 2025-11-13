#pragma once
#include "StaticActor.h"

class CUI : public CStaticActor
{
	GENERATE_OBJECT(CUI)
		DONTCOPY(CUI)
public:
	CUI()
	{
		InteractionComponent = AddComponent<CInteractionComponent>();
	}
	virtual ~CUI() = default;

public:
	void InitalizeBasicButtonUI(const std::wstring& InBasicImagePath, const std::wstring& InMouseOnImagePath
		, const std::wstring& InMouseClickImagePath, std::function<void()> InButtonEvent)
	{
		SpriteRenderComponent->SetDiffuseImage(InBasicImagePath);
		InteractionComponent->SetMouseExitEvent([this, InBasicImagePath]()->void
			{
				SpriteRenderComponent->SetDiffuseImage(InBasicImagePath);
			});
		InteractionComponent->SetMouseEnterEvent([this, InMouseOnImagePath]()->void
			{
				SpriteRenderComponent->SetDiffuseImage(InMouseOnImagePath);
			});
		InteractionComponent->SetMouseFocusEvent([this, InBasicImagePath, InMouseClickImagePath, InButtonEvent]()->void
			{
				if (LClicked() && InMouseClickImagePath.empty() == false)
					SpriteRenderComponent->SetDiffuseImage(InMouseClickImagePath);
				else if (LReleased())
				{
					SpriteRenderComponent->SetDiffuseImage(InBasicImagePath);
					if (InButtonEvent)
						InButtonEvent();
				}
			});
	}

private:
	

};

