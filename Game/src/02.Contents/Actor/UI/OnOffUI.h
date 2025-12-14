#pragma once
#include "UI.h"

class COnOffUI : public CUI
{
	GENERATE_OBJECT(COnOffUI)
public:
	COnOffUI() = default;
	~COnOffUI() = default;

public:
	void InitalizeOnOffUI(const std::wstring& InOffImagePath, const std::wstring& InOnImagePath)
	{
		OffImagePath = InOffImagePath;
		OnImagePath = InOnImagePath;
		Off();
	}
	void ChangeState()
	{
		if (bOn)
			Off();
		else
			On();
	}

private:
	void Off()
	{
		SpriteRenderComponent->SetDiffuseImage(OffImagePath);
		bOn = false;
	}
	void On()
	{
		SpriteRenderComponent->SetDiffuseImage(OnImagePath);
		bOn = true;
	}
	
private:
	std::wstring OffImagePath;
	std::wstring OnImagePath;
	bool bOn = false;
};

