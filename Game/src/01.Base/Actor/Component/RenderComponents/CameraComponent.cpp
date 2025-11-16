#include "pch.h"
#include "CameraComponent.h"

#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderResourceLoader.h"

CCameraComponent::CCameraComponent()
	: CameraConst{}
	, bUpdateCameraConst(false)
{
	RenderStateObject.MountConstBuffer(EShaderType::VertexShader, 1
		, CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(CameraConst)));
}

void CCameraComponent::Render(CSpriteRenderer& InRenderer)
{
	if (bUpdateCameraConst)
		InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::VertexShader, 1, &CameraConst, sizeof(CameraConst));

	InRenderer.RenderObject(RenderStateObject);

	bUpdateCameraConst = false;
}