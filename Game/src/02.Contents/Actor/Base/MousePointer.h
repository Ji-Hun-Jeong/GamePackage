#pragma once
#include "01.Base/Object/Actor.h"
#include "06.Interaction/MouseManager.h"

class CMousePointer : public CActor
{
	GENERATE_OBJECT()
public:
	CMousePointer()
		: MousePosition(nullptr)
	{}
	~CMousePointer() = default;

public:
	void Initalize() override
	{
		CActor::Initalize();
		SetRenderComponent();
	}
	void SetMousePosition(CMousePosition* InMousePosition) { MousePosition = InMousePosition; }
	const CMousePosition* GetMousePosition() const { return MousePosition; }

	void FinalUpdate() override
	{
		CActor::FinalUpdate();
		MousePosition->SetMousePosition(
			Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y));
	}

	void SetMouseImageFromDialog(EKeyType InKeyType, class CWindowIOManager& InWindowIOManager);

private:
	CMousePosition* MousePosition;

};

