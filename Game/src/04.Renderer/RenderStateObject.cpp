#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>

void CRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext)
{
	if (Mesh && Material && PSO)
	{
		while (UpdateBufferIndexs.empty() == false)
		{
			uint32_t Index = UpdateBufferIndexs.front();
			UpdateBufferIndexs.pop();
			InContext.CopyBuffer(*VertexConstBuffers[Index], VertexBufferMapResources[Index]);
		};
		if (VertexConstBuffers.size())
			InContext.VSSetConstantBuffers(0, uint32_t(VertexConstBuffers.size()), VertexConstBuffers.front());
		PSO->BindToPipeline(InContext);
		Material->BindToPipeline(InContext);
		Mesh->BindToPipeline(InContext);
	}
}
