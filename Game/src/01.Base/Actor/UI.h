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
		RenderComponent->SetDiffuseImage(InBasicImagePath);
		InteractionComponent->SetMouseExitEvent([this, InBasicImagePath]()->void
			{
				RenderComponent->SetDiffuseImage(InBasicImagePath);
			});
		InteractionComponent->SetMouseEnterEvent([this, InMouseOnImagePath]()->void
			{
				RenderComponent->SetDiffuseImage(InMouseOnImagePath);
			});
		InteractionComponent->SetMouseFocusEvent([this, InBasicImagePath, InMouseClickImagePath, InButtonEvent]()->void
			{
				if (LClicked() && InMouseClickImagePath.empty() == false)
					RenderComponent->SetDiffuseImage(InMouseClickImagePath);
				else if (LReleased())
				{
					RenderComponent->SetDiffuseImage(InBasicImagePath);
					if (InButtonEvent)
						InButtonEvent();
				}
			});
	}

private:
	

};

