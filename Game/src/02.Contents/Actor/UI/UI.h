#pragma once
#include "01.Base/Actor/StaticActor.h"

class CUI : public CStaticActor
{
	friend class CUIManager;
	GENERATE_OBJECT(CUI)
		DONTCOPY(CUI)
public:
	CUI()
	{
		SpriteRenderComponent->SetUIPass();
	}
	virtual ~CUI() = default;

public:
	void Activate(bool bInActivate) override
	{
		CStaticActor::Activate(bInActivate);
		if (SpriteRenderComponent)
			SpriteRenderComponent->SetRender(bInActivate);
		bInteraction = bInActivate;
	}

	void SetMouseEnterEvent(std::function<void()> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void()> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseOnEvent(std::function<void()> InMouseOnEvent) { MouseOnEvent = InMouseOnEvent; }
	void SetMouseFocusEvent(std::function<void()> InMouseFocusEvent) { MouseFocusEvent = InMouseFocusEvent; }

	void SetBaseImagePath(const std::wstring& InPath)
	{
		BaseImagePath = InPath;
		SpriteRenderComponent->SetDiffuseImage(BaseImagePath);
	}
	void SetHoverImagePath(const std::wstring& InPath) { HoverImagePath = InPath; }
	void SetClickedImagePath(const std::wstring& InPath) { ClickedImagePath = InPath; }

	void ApplyBaseImage() { SpriteRenderComponent->SetDiffuseImage(BaseImagePath); }
	void ApplyHoverImage() { SpriteRenderComponent->SetDiffuseImage(HoverImagePath); }
	void ApplyClickedImage() { SpriteRenderComponent->SetDiffuseImage(ClickedImagePath); }

	void SetUIImages(const std::wstring& InBase, const std::wstring& InHover = L"", const std::wstring& InClicked = L"")
	{
		BaseImagePath = InBase;
		HoverImagePath = InHover;
		ClickedImagePath = InClicked;
		SpriteRenderComponent->SetDiffuseImage(BaseImagePath);
	}

	void AttachChildUI(CUI& InChildUI)
	{
		if (InChildUI.OwnerUI == this)
			return;
		if (InChildUI.OwnerUI)
			InChildUI.OwnerUI->DetachChildUI(InChildUI);

		Attach(&InChildUI);
		ChildUIs.push_back(&InChildUI);
		InChildUI.OwnerUI = this;
	}
	void DetachChildUI(CUI& InChildUI)
	{
		if (&InChildUI == this)
			return;
		if (InChildUI.OwnerUI != this)
			return;

		Detach(&InChildUI);
		for (auto Iter = ChildUIs.begin(); Iter != ChildUIs.end(); ++Iter)
		{
			if (*Iter == &InChildUI)
			{
				ChildUIs.erase(Iter);
				InChildUI.OwnerUI = nullptr;
				break;
			}
		}
	}

	CUI* GetOwnerUI() const { return OwnerUI; }
	void SetUILayer(uint32_t InUILayer)
	{
		UILayer = InUILayer;
	}
	uint32_t GetUILayer() const { return UILayer; }
	uint32_t GetFinalUILayer() const { return FinalUILayer; }

private:
	void SetFinalUILayer(uint32_t InFinalUILayer)
	{
		FinalUILayer = InFinalUILayer;
		if (SpriteRenderComponent)
			SpriteRenderComponent->SetLayer(UILayer);
	}

private:
	std::function<void()> MouseEnterEvent = nullptr;
	std::function<void()> MouseExitEvent = nullptr;
	std::function<void()> MouseOnEvent = nullptr;
	std::function<void()> MouseFocusEvent = nullptr;

	bool bInteraction = true;

	std::vector<CUI*> ChildUIs;
	CUI* OwnerUI = nullptr;

	uint32_t UILayer = 0;
	uint32_t FinalUILayer = 0;

	std::wstring BaseImagePath;
	std::wstring HoverImagePath;
	std::wstring ClickedImagePath;

};

