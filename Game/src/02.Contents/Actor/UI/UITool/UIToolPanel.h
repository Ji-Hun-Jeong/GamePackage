#pragma once
#include "pch.h"
#include "01.Base/Object/UI.h"

class CUIToolPanel : public CUI
{
	GENERATE_OBJECT()
public:
	CUIToolPanel() {}
	~CUIToolPanel() {}

public:
	void Initalize() override
	{
		CUI::Initalize();
		GetRenderComponent()->SetPSO(EPSOType::Mark);
	}

public:
	CUI* PlaceUIOnToolPanel(const std::wstring& InUIImagePath, const Vector2& InMouseWorldPosition);
	[[nodiscard]] size_t AddCreateChildUIEvent(std::function<void(CUI&)> CreateChildUIEvent) 
	{ 
		assert(CreateChildUIEvent); 
		CreateChildUIEvents.push_back(CreateChildUIEvent); 
		return CreateChildUIEvents.size() - 1;
	}
	void RemoveCreateChildUIEvent(size_t InIndex)
	{
		auto Iter = CreateChildUIEvents.begin();
		for (size_t i = 0; i < InIndex; ++i)
			++Iter;
		CreateChildUIEvents.erase(Iter);
	}

private:
	std::vector<std::function<void(CUI&)>> CreateChildUIEvents;

};
