#include "pch.h"
#include "Camera.h"

#include "04.Renderer/SpriteRenderer.h"

CCamera::CCamera()
{
}

void CCamera::RenderActor(CSpriteRenderer& InRenderer)
{
	const Vector3& Position = Transform->GetWorldPosition();
	const Vector3& Rotation = Transform->GetRotation();
	const Vector3& Scale = Transform->GetScale();

	InRenderer.SetViewInfo(Position, Rotation, Scale, false);
}
