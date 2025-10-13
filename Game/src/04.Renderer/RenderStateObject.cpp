#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>

void CBasicRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext) const
{
	InContext.IASetInputLayout(*InputLayout);
	InContext.VSSetShader(*VertexShader);

	Material->BindToPipeline(InContext);
	Mesh->BindToPipeline(InContext);
}
