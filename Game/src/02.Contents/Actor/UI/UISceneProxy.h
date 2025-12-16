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
	std::vector<CUI*> UIWindows;

};

