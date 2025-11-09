#pragma once
#include "01.Base/Actor/StaticActor.h"

class CBackGround : public CStaticActor
{
	GENERATE_OBJECT(CBackGround)
	DONTCOPY(CBackGround)
public:
	CBackGround()
	{
		PixelCollider = AddComponent<CPixelCollider>();
	}
	~CBackGround() = default;

public:
	void InitalizeBackGround(const std::wstring& InBackGroundImagePath)
	{
		RenderComponent->SetDiffuseImage(InBackGroundImagePath);
		PixelCollider->SetPixelMap(EPixelMapType::OtherMap, InBackGroundImagePath);
	}

private:

};

