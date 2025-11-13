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
	void FinalUpdate() override
	{
		CActor::FinalUpdate();

		uint32_t ScreenWidth = CWindowManager::GetInst().GetScreenWidth();
		uint32_t ScreenHeight = CWindowManager::GetInst().GetScreenHeight();
		Transform->SetScale(Vector3(float(ScreenWidth), float(ScreenHeight), 1.0f));
	}
	void CaptureSnapShot() override;
	void SetupInputActionValue(class CInputActionValueCollector& InInputActionValueCollector) override;

private:
	std::unique_ptr<class CInputActionValue> LeftMoveActionValue;
	std::unique_ptr<class CInputActionValue> RightMoveActionValue;
	std::unique_ptr<class CInputActionValue> UpMoveActionValue;
	std::unique_ptr<class CInputActionValue> DownMoveActionValue;

	CCameraComponent* CameraComponent;
};

