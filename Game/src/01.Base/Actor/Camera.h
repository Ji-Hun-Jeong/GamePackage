#pragma once
#include "Actor.h"
#include "Component/RenderComponents/CameraComponent.h"

class CCamera : public CActor
{
	GENERATE_OBJECT(CCamera)
	DONTCOPY(CCamera)
public:
	CCamera();
	~CCamera() = default;

public:
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		if (GetKey(EKeyType::Left, EButtonState::Hold))
			Transform->Move(Vector3(-1.0f, 0.0f, 0.0f));
		if (GetKey(EKeyType::Right, EButtonState::Hold))
			Transform->Move(Vector3(1.0f, 0.0f, 0.0f));
		if (GetKey(EKeyType::Up, EButtonState::Hold))
			Transform->Move(Vector3(0.0f, 1.0f, 0.0f));
		if (GetKey(EKeyType::Down, EButtonState::Hold))
			Transform->Move(Vector3(0.0f, -1.0f, 0.0f));
	}
	void FinalUpdate() override
	{
		CActor::FinalUpdate();

		uint32_t ScreenWidth = CWindowManager::GetInst().GetScreenWidth();
		uint32_t ScreenHeight = CWindowManager::GetInst().GetScreenHeight();
		Transform->SetScale(Vector3(float(ScreenWidth), float(ScreenHeight), 1.0f));
	}

private:
	CCameraComponent* CameraComponent;

};

