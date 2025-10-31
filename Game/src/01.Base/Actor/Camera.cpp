#include "pch.h"
#include "Camera.h"
#include "05.Input/InputActionManager.h"

CCamera::CCamera()
	: LeftMoveActionValue(nullptr)
	, RightMoveActionValue(nullptr)
	, UpMoveActionValue(nullptr)
	, DownMoveActionValue(nullptr)
{
	LeftMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f)); });
	LeftMoveActionValue->AddKeyCondition({ EKeyType::Left, EButtonState::Hold });

	RightMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f)); });
	RightMoveActionValue->AddKeyCondition({ EKeyType::Right, EButtonState::Hold });

	UpMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f)); });
	UpMoveActionValue->AddKeyCondition({ EKeyType::Up, EButtonState::Hold });

	DownMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f)); });
	DownMoveActionValue->AddKeyCondition({ EKeyType::Down, EButtonState::Hold });

	RenderComponent = AddComponent<CRenderComponent>();

	RenderComponent->SetVertexConstBufferData(1, sizeof(CameraConst));

	Transform->SetSpeed(2.0f);
}

void CCamera::CaptureSnapShot(uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	CActor::CaptureSnapShot(InScreenWidth, InScreenHeight);

	Transform->SetScale(Vector3(float(InScreenWidth), float(InScreenHeight), 1.0f));

	CameraConst.ViewProj = (Transform->GetNDCModelMatrix(InScreenWidth, InScreenHeight).Invert()).Transpose();

	RenderComponent->UpdateVertexConstBufferData(1, &CameraConst, sizeof(CameraConst));
}

void CCamera::SetupInputActionValue(CInputActionValueCollector& InInputActionValueCollector)
{
	InInputActionValueCollector.PushInputActionValue(*LeftMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*RightMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*UpMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*DownMoveActionValue);
}
