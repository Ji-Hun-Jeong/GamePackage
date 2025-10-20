#pragma once
#include "Actor.h"

class CUI : public CActor
{
	GENERATE_OBJECT()
	DONTCOPY(CUI)
public:
	CUI() = default;
	virtual ~CUI() = default;

public:
	virtual void Initalize() override;

	void InitalizeBasicButtonUI(const std::wstring& InBasicImagePath, const std::wstring& InMouseOnImagePath
	, const std::wstring& InMouseClickImagePath = L"", const std::wstring& InMouseReleaseImagePath = L""
		, std::function<void()> InMouseReleaseEvent = nullptr)
	{
		GetRenderComponent()->SetDiffuseImage(InBasicImagePath);
		GetInteractionComponent()->SetMouseExitEvent([this, InBasicImagePath]()->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InBasicImagePath);
			});
		GetInteractionComponent()->SetMouseEnterEvent([this, InMouseOnImagePath]()->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InMouseOnImagePath);
			});
		GetInteractionComponent()->SetMouseClickEvent([this, InMouseClickImagePath]()->void
			{
				if (GetRenderComponent())
					if (InMouseClickImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseClickImagePath);
			});
		GetInteractionComponent()->SetMouseReleaseEvent([this, InMouseReleaseImagePath, InMouseReleaseEvent]()->void
			{
				if (GetRenderComponent())
					if (InMouseReleaseImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseReleaseImagePath);
				if (InMouseReleaseEvent)
					InMouseReleaseEvent();
			});
	}

private:

};

