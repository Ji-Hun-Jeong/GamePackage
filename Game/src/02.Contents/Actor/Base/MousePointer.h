#pragma once
#include "01.Base/Actor/StaticActor.h"

class CMousePointer : public CStaticActor
{
	GENERATE_OBJECT(CMousePointer)
public:
	CMousePointer() = default;
	~CMousePointer() = default;

public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);
		Vector3 Position = Transform->GetFinalPosition();

		/*if (CMouseInteracter::GetCurrentMouseX() != Position.x || CMouseInteracter::GetCurrentMouseY() != Position.y)
		{
			Position.x = float(CMouseManager::GetInst().GetMousePosition().x);
			Position.y = float(CMouseManager::GetInst().GetMousePosition().y);
			Transform->SetPosition(Position);
		}*/
	}
	void SetMouseImageFromDialog(class CWindowManager& InWindowManager);

private:

};

