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
		//SpriteRenderComponent->SetUIPass();
	}
	virtual ~CUI() = default;

public:
	void Activate(bool bInActivate) override
	{
		CStaticActor::Activate(bInActivate);
		SpriteRenderComponent->SetRender(bInActivate);
		bInteraction = bInActivate;
	}
	void InitalizeBasicButtonUI(const std::wstring& InBasicImagePath, const std::wstring& InMouseOnImagePath
		, const std::wstring& InMouseClickImagePath, std::function<void()> InButtonEvent)
	{
		SpriteRenderComponent->SetDiffuseImage(InBasicImagePath);
		MouseExitEvent = [this, InBasicImagePath]()->void{SpriteRenderComponent->SetDiffuseImage(InBasicImagePath);};
		MouseEnterEvent = ([this, InMouseOnImagePath]()->void{SpriteRenderComponent->SetDiffuseImage(InMouseOnImagePath);});
		MouseFocusEvent = ([this, InBasicImagePath, InMouseClickImagePath, InButtonEvent]()->void
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
	
	void SetRectUI(uint32_t InLayer)
	{
		SpriteRenderComponent->SetMesh(CAssetLoader::GetInst().GetMeshData("LineSquareMesh"));
		SpriteRenderComponent->SetPSO(EPSOType::Line);
		SpriteRenderComponent->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
		SpriteRenderComponent->SetLayer(InLayer);
	}

	void SetInteraction(bool bInInteraction) { bInteraction = bInInteraction; }
	void SetEndInteractionCheck(bool bInEndInteractionCheck) { bEndInteractionCheck = bInEndInteractionCheck; }
	void SetMouseEnterEvent(std::function<void()> InMouseEnterEvent) { MouseEnterEvent = InMouseEnterEvent; }
	void SetMouseExitEvent(std::function<void()> InMouseExitEvent) { MouseExitEvent = InMouseExitEvent; }
	void SetMouseOnEvent(std::function<void()> InMouseOnEvent) { MouseOnEvent = InMouseOnEvent; }
	void SetMouseFocusEvent(std::function<void()> InMouseFocusEvent) { MouseFocusEvent = InMouseFocusEvent; }

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

private:
	std::function<void()> MouseEnterEvent = nullptr;
	std::function<void()> MouseExitEvent = nullptr;
	std::function<void()> MouseOnEvent = nullptr;
	std::function<void()> MouseFocusEvent = nullptr;

	bool bInteraction = true;
	bool bEndInteractionCheck = false;

	std::vector<CUI*> ChildUIs;
	CUI* OwnerUI = nullptr;
};

