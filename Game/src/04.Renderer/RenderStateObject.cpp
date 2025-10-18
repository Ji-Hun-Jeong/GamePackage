#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>

void CRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext)
{
	// ConstBuffer Update And Setting
	if (VertexConstBuffers.size())
	{
		while (BufferMapInstances.empty() == false)
		{
			const CBufferMapInstance* BufferMapInstance = BufferMapInstances.front();
			BufferMapInstances.pop();
			InContext.CopyBuffer(*VertexConstBuffers[BufferMapInstance->MappingIndex], BufferMapInstance->MapDataPoint, BufferMapInstance->DataSize);
		};
		InContext.VSSetConstantBuffers(StartSlot, uint32_t(VertexConstBuffers.size()), VertexConstBuffers);
	}

	if (Mesh && Image && PSO)
	{
		PSO->BindToPipeline(InContext);
		Image->BindToPipeline(InContext);
		Mesh->BindToPipeline(InContext);
	}
}
