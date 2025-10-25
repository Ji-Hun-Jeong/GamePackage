#pragma once
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Component/Component.h"
#include "06.Interaction/MouseManager.h"

class CMousePointer : public CActor
{
	GENERATE_OBJECT(CMousePointer)
public:
	CMousePointer()
		: MouseManager(nullptr)
	{}
	~CMousePointer() = default;

public:
	void Initalize() override;
	void Destroy() override
	{
		CActor::Destroy();
		MouseManager->SetMousePositionInstance(nullptr);
	}
	void FinalUpdate() override
	{
		CActor::FinalUpdate();

		MousePositionInstance->SetMousePosition(
			Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y));
	}
	void SetMouseManager(CMouseManager& InMouseManager)
	{
		MouseManager = &InMouseManager;
		MousePositionInstance = std::make_unique<CMousePositionInstance>();
		MouseManager->SetMousePositionInstance(MousePositionInstance.get());
	}

	void SetMouseImageFromDialog(class CWindowIOManager& InWindowIOManager);

	Vector2 GetMousePosition() const { return MousePositionInstance->GetMousePosition(); }

private:
	CMouseManager* MouseManager;
	std::unique_ptr<CMousePositionInstance> MousePositionInstance;

};

