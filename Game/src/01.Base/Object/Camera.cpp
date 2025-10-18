#include "pch.h"
#include "Camera.h"
#include "05.Input/InputActionManager.h"

CCamera::~CCamera()
{
	CActor::Destroy();
	LeftMoveActionValue->Destroy();
	RightMoveActionValue->Destroy();
	UpMoveActionValue->Destroy();
	DownMoveActionValue->Destroy();
}

void CCamera::Initalize()
{
	CActor::Initalize();
	SetRenderComponent();

	GetRenderComponent()->AddVertexConstBuffer(sizeof(CameraConst));

	GetTransform()->SetSpeed(0.005f);
}

void CCamera::CaptureSnapShot()
{
	if (GetTransform()->OnVariation() == false)
		return;

	GetTransform()->CalculateModelMatrix();

	CameraConst.ViewProj = GetTransform()->GetModelMatrix().Invert();

	GetRenderComponent()->UpdateVertexConstBuffer(1, &CameraConst, sizeof(CameraConst));
}

void CCamera::SetInputAction(CInputActionManager& InInputActionManager)
{
	LeftMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::A, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f));
		});
	RightMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::D, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f));
		});
	UpMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::W, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f));
		});
	DownMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::S, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f));
		});
}
