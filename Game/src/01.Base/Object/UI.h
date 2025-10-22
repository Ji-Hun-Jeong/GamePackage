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
		GetInteractionComponent()->SetMouseExitEvent([this, InBasicImagePath](const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InBasicImagePath);
			});
		GetInteractionComponent()->SetMouseEnterEvent([this, InMouseOnImagePath](const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					GetRenderComponent()->SetDiffuseImage(InMouseOnImagePath);
			});
		GetInteractionComponent()->SetMouseClickEvent([this, InMouseClickImagePath](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					if (InMouseClickImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseClickImagePath);
			});
		GetInteractionComponent()->SetMouseReleaseEvent([this, InMouseReleaseImagePath, InMouseReleaseEvent](EKeyType InKeyType, const Vector2& InMousePosition)->void
			{
				if (GetRenderComponent())
					if (InMouseReleaseImagePath.empty() == false)
						GetRenderComponent()->SetDiffuseImage(InMouseReleaseImagePath);
				if (InMouseReleaseEvent)
					InMouseReleaseEvent(InKeyType, InMousePosition);
			});
	}

private:

};

