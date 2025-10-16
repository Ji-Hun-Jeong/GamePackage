#include "pch.h"
#include "Image.h"
#include <Renderer/Base/RenderContext.h>

void CImage::BindToPipeline(Graphics::CRenderContext& InContext)
{
	InContext.PSSetShaderResource(StartSlot, *ShaderResourceView.get());
	InContext.VSSetConstantBuffer(StartSlot, *MeshResizeConstBuffer.get());
}
