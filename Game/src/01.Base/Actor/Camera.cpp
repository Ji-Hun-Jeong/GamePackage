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

	RenderComponent->SetConstBuffer(EShaderType::VertexShader, 1, sizeof(CameraConst));

	Transform->SetSpeed(2.0f);
}

void CCamera::CaptureSnapShot()
{
	CActor::CaptureSnapShot();
	
	if (Transform->OnVariation())
	{
		uint32_t ScreenWidth = CWindowManager::GetScreenWidth();
		uint32_t ScreenHeight = CWindowManager::GetScreenHeight();
		CameraConst.ViewProj = (Transform->GetNDCModelMatrix(ScreenWidth, ScreenHeight).Invert()).Transpose();
		CameraConst.ScreenWidth = ScreenWidth;
		CameraConst.ScreenHeight = ScreenHeight;

		RenderComponent->UpdateConstBuffer(EShaderType::VertexShader, 1, &CameraConst, sizeof(CameraConst));
		Transform->ClearVariation();
	}
}

void CCamera::SetupInputActionValue(CInputActionValueCollector& InInputActionValueCollector)
{
	InInputActionValueCollector.PushInputActionValue(*LeftMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*RightMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*UpMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*DownMoveActionValue);
}
