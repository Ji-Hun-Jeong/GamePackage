#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
	: CameraComponent(nullptr)
{
	CameraComponent = AddComponent<CCameraComponent>();

	Transform->SetSpeed(5.0f);
}