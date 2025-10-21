#pragma once
#include "01.Base/Object/Actor.h"
#include "06.Interaction/MouseManager.h"

class CMousePointer : public CActor
{
	GENERATE_OBJECT()
public:
	CMousePointer() = default;
	~CMousePointer() = default;

public:
	void SetMousePosition(CMousePosition* InMousePosition) { MousePosition = InMousePosition; }
	void FinalUpdate() override
	{
		CActor::FinalUpdate();
		MousePosition->SetMousePosition(
			Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y));
	}

private:
	CMousePosition* MousePosition;

};

