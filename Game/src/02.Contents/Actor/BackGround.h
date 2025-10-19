#pragma once
#include "01.Base/Object/Actor.h"

class CBackGround : public CActor
{
	GENERATE_OBJECT()
	DONTCOPY(CBackGround)
public:
	CBackGround() = default;
	~CBackGround() = default;

public:
	void InitalizeBackGround(const std::wstring& InBackGroundImagePath)
	{
		SetRenderComponent();
		GetRenderComponent()->SetImage(InBackGroundImagePath);
	}

private:

};

