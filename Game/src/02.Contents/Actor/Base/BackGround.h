#pragma once
#include "01.Base/Object/Actor.h"

class CBackGround : public CActor
{
	GENERATE_OBJECT(CBackGround)
	DONTCOPY(CBackGround)
public:
	CBackGround() = default;
	~CBackGround() = default;

public:
	void InitalizeBackGround(const std::wstring& InBackGroundImagePath)
	{
		/*SetRenderComponent();
		GetRenderComponent()->SetDiffuseImage(InBackGroundImagePath);*/
	}

private:

};

