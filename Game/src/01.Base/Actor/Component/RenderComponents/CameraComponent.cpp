#include "pch.h"
#include "CameraComponent.h"

#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderResourceLoader.h"

CCameraComponent::CCameraComponent()
{
}

void CCameraComponent::Render(CSpriteRenderer& InRenderer, const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale)
{
	InRenderer.SetViewInfo(InPosition, InRotation, InScale, false);
}