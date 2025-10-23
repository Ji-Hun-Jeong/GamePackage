#pragma once
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Component/Component.h"
#include "06.Interaction/MouseManager.h"

class CMousePositionComponent : public CComponent
{
	GENERATE_OBJECT()
public:
	CMousePositionComponent() = default;
	~CMousePositionComponent() = default;

public:
	void Destroy() override
	{
		CComponent::Destroy();
		MouseManager->SetMousePositionInstance(nullptr);
	}
	void SetMouseManager(CMouseManager& InMouseManager) 
	{ 
		MouseManager = &InMouseManager;
		MousePositionInstance = std::make_unique<CMousePositionInstance>();
		MouseManager->SetMousePositionInstance(MousePositionInstance.get());
	}
	void SetMousePosition(const Vector2& InMousePosition)
	{
		MousePositionInstance->SetMousePosition(InMousePosition);
	}

private:
	CMouseManager* MouseManager;
	std::unique_ptr<CMousePositionInstance> MousePositionInstance;

};

class CMousePointer : public CActor
{
	GENERATE_OBJECT()
public:
	CMousePointer()
		: MousePositionComponent(nullptr)
	{}
	~CMousePointer() = default;

public:
	void Initalize() override;

	void FinalUpdate() override
	{
		CActor::FinalUpdate();
		MousePositionComponent->SetMousePosition(
			Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y));
	}

	void SetMouseImageFromDialog(EKeyType InKeyType, class CWindowIOManager& InWindowIOManager);

private:
	CMousePositionComponent* MousePositionComponent;

};

