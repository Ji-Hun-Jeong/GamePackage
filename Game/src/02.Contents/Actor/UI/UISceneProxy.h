#pragma once
#include "UI.h"

class CUISceneProxy
{
public:
	CUISceneProxy() = default;
	~CUISceneProxy() = default;

public:
	void AddUIWindow(CUI& InUIWindow)
	{
		UIWindows.push_back(&InUIWindow);
	}
	void ArrangeLayer()
	{
		for (size_t i = 0; i < UIWindows.size(); ++i)
		{
			CUI* UIWindow = UIWindows[i];
			UIWindow->SetUILayer(i * 100);
			GetFinalLayer(*UIWindow, UIWindow->UILayer);
		}
	}
	void WindowToFront(CUI& InUIWindow)
	{
		auto it = std::find(UIWindows.begin(), UIWindows.end(), &InUIWindow);

		if (it != UIWindows.end())
		{
			UIWindows.erase(it);            
			UIWindows.push_back(&InUIWindow); 
		}
	}

private:
	void GetFinalLayer(CUI& InOwnerUI, uint32_t InOwnerLayer)
	{
		for (auto ChildUI : InOwnerUI.ChildUIs)
		{
			ChildUI->SetUILayer(InOwnerLayer + 1);
			GetFinalLayer(*ChildUI, ChildUI->UILayer);
		}
	}

private:
	std::vector<CUI*> UIWindows;

};

