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
		GetRenderComponent()->SetImage(InBasicImagePath);
		GetInteractionComponent()->SetMouseExitEvent([this, InBasicImagePath]()->void
			{
				GetRenderComponent()->SetImage(InBasicImagePath);
			});
		GetInteractionComponent()->SetMouseEnterEvent([this, InMouseOnImagePath]()->void
			{
				GetRenderComponent()->SetImage(InMouseOnImagePath);
			});
		GetInteractionComponent()->SetMouseClickEvent([this, InMouseClickImagePath]()->void
			{
				if (InMouseClickImagePath.empty() == false)
					GetRenderComponent()->SetImage(InMouseClickImagePath);
			});
		GetInteractionComponent()->SetMouseReleaseEvent([this, InMouseReleaseImagePath, InMouseReleaseEvent]()->void
			{
				if (InMouseReleaseImagePath.empty() == false)
					GetRenderComponent()->SetImage(InMouseReleaseImagePath);
				if (InMouseReleaseEvent)
					InMouseReleaseEvent();
			});
	}

private:

};

