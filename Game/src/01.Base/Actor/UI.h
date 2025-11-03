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
		, const std::wstring& InMouseClickImagePath = L"", const std::wstring& InMouseReleaseImagePath = L""
		, std::function<void(EKeyType, const Vector2&)> InMouseReleaseEvent = nullptr)
	{
		RenderComponent->SetDiffuseImage(InBasicImagePath);
		InteractionComponent->SetMouseExitEvent([this, InBasicImagePath](const Vector2& InMousePosition)->void
			{
				RenderComponent->SetDiffuseImage(InBasicImagePath);
			});
		InteractionComponent->SetMouseEnterEvent([this, InMouseOnImagePath](const Vector2& InMousePosition)->void
			{
				RenderComponent->SetDiffuseImage(InMouseOnImagePath);
			});
		/*InteractionComponent->SetMouseOnEvent([this, InMouseClickImagePath](const Vector2& InMousePosition)->void
			{
				if (InMouseClickImagePath.empty() == false)
					RenderComponent->SetDiffuseImage(InMouseClickImagePath);
			});
		InteractionComponent->SetMouseEnterEvent([this, InMouseReleaseImagePath, InMouseReleaseEvent](const Vector2& InMousePosition)->void
			{
				if (InMouseReleaseImagePath.empty() == false)
					RenderComponent->SetDiffuseImage(InMouseReleaseImagePath);
			});*/
	}

private:

};

