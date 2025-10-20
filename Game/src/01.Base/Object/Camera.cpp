#include "pch.h"
#include "Camera.h"
#include "05.Input/InputActionManager.h"

CCamera::~CCamera()
{

}

void CCamera::Initalize()
{
	CActor::Initalize();
	SetRenderComponent();

	GetRenderComponent()->AddVertexConstBuffer(sizeof(CameraConst));

	GetTransform()->SetSpeed(3.5f);
}

void CCamera::BeginPlay()
{
	CActor::BeginPlay();
	GetTransform()->SetScale(Vector3(float(CameraConst.ScreenWidth), float(CameraConst.ScreenHeight), 1.0f));
}

void CCamera::CaptureSnapShot()
{
	if (GetTransform()->OnVariation() == false)
		return;

	GetTransform()->SetVariation(false);

	CameraConst.ViewProj = GetRenderComponent()->GetModelMatrix(
		GetTransform()->GetFinalPosition(), GetTransform()->GetRotation(), GetTransform()->GetScale()).Invert();

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

void CCamera::Destroy()
{
	CActor::Destroy();
	LeftMoveActionValue->Destroy();
	RightMoveActionValue->Destroy();
	UpMoveActionValue->Destroy();
	DownMoveActionValue->Destroy();
}
