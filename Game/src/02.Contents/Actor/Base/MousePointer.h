#pragma once
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Component/Component.h"
#include "06.Interaction/MouseManager.h"

class CMousePointer : public CActor
{
	GENERATE_OBJECT(CMousePointer)
public:
	CMousePointer()
		: MousePositionInstance(std::make_unique<CMousePositionInstance>())
	{}
	~CMousePointer() = default;

public:
	void Initalize() override;
	void EndPlay() override
	{
		CActor::EndPlay();
	}
	void FinalUpdate() override
	{
		CActor::FinalUpdate();

		MousePositionInstance->SetMousePosition(
			Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y));
	}

	void SetMouseImageFromDialog(class CWindowIOManager& InWindowIOManager);
	Vector2 GetMousePosition() const { return MousePositionInstance->GetMousePosition(); }

	const CMousePositionInstance* GetMousePositionInstance() const { return MousePositionInstance.get(); }

private:
	std::unique_ptr<CMousePositionInstance> MousePositionInstance;

};

