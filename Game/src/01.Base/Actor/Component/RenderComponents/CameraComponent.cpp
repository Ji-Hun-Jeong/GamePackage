#include "pch.h"
#include "CameraComponent.h"

#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderResourceLoader.h"

CCameraComponent::CCameraComponent()
	: CameraConst{}
	, bUpdateCameraConst(false)
{
	ViewBuffer = CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(CameraConst));
}

void CCameraComponent::Render(CSpriteRenderer& InRenderer)
{
	if (bUpdateCameraConst)
		InRenderer.UpdateBuffer(*ViewBuffer.get(), &CameraConst, sizeof(CameraConst));

	InRenderer.SetCameraOffset(Vector2(Position.x, Position.y));
	InRenderer.SetConstBuffer(EShaderType::VertexShader, 1, *ViewBuffer.get());

	bUpdateCameraConst = false;
}