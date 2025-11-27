#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
	: CameraComponent(nullptr)
{
	CameraComponent = AddComponent<CCameraComponent>();
	RenderComponent = CameraComponent;

	Transform->SetSpeed(5.0f);
}

void CCamera::CaptureSnapShot()
{
	CActor::CaptureSnapShot();
	
	if (Transform->OnVariation())
	{
		uint32_t ScreenWidth = CWindowManager::GetInst().GetScreenWidth();
		uint32_t ScreenHeight = CWindowManager::GetInst().GetScreenHeight();
		const Vector3& Position = Transform->GetFinalPosition();
		const Vector3& Rotation = Transform->GetRotation();
		const Vector3& Scale = Transform->GetScale();

		CameraComponent->UpdateViewToNDC(Position, Rotation, Scale, ScreenWidth, ScreenHeight);
		Transform->ClearVariation();
	}
}