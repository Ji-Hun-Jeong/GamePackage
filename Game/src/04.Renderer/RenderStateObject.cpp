#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>

void CRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext)
{
	if (Mesh && Material && PSO)
	{
		while (BufferMapInstances.empty() == false)
		{
			const CBufferMapInstance* BufferMapInstance = BufferMapInstances.front();
			BufferMapInstances.pop();
			InContext.CopyBuffer(*VertexConstBuffers[BufferMapInstance->MappingIndex], BufferMapInstance->MapDataPoint, BufferMapInstance->DataSize);
		};
		if (VertexConstBuffers.size())
			InContext.VSSetConstantBuffers(0, uint32_t(VertexConstBuffers.size()), VertexConstBuffers);
		PSO->BindToPipeline(InContext);
		Material->BindToPipeline(InContext);
		Mesh->BindToPipeline(InContext);
	}
}
