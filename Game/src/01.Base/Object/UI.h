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
		, std::function<void(EKeyType, const Vector2&)> InMouseReleaseEvent = nullptr)
	{
		GetRenderComponent()->SetDiffuseImage(InBasicImagePath);
		GetInteractionComponent()->SetInteracterDeactivateEvent([this, InBasicImagePath](const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InBasicImagePath);
			});
		GetInteractionComponent()->SetInteracterActivateEvent([this, InMouseOnImagePath](const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InMouseOnImagePath);
			});
		GetInteractionComponent()->SetInteracterFocusMouseClickEvent([this, InMouseClickImagePath](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent() && InMouseClickImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseClickImagePath);
			});
		GetInteractionComponent()->SetInteracterFocusMouseReleaseEvent([this, InMouseReleaseImagePath, InMouseReleaseEvent](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent() && InMouseReleaseImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseReleaseImagePath);
				if (InMouseReleaseEvent)
					InMouseReleaseEvent(InKeyType, InMousePosition);
			});
	}

private:

};

