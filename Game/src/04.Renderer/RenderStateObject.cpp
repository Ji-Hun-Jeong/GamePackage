#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>

void CRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext)
{
	while (BufferMapInstances.empty() == false)
	{
		const CBufferMapInstance* BufferMapInstance = BufferMapInstances.front();
		BufferMapInstances.pop();
		InContext.CopyBuffer(*VertexConstBuffers[BufferMapInstance->MappingIndex], BufferMapInstance->MapDataPoint, BufferMapInstance->DataSize);
	};

	if (VertexConstBuffers.size())
		InContext.VSSetConstantBuffers(VertexConstBufferStartSlot, uint32_t(VertexConstBuffers.size()), VertexConstBuffers);

	if (Mesh && PSO)
	{
		PSO->BindToPipeline(InContext);

		InContext.IASetVertexBuffer(Mesh->GetVertexBuffer(), &Mesh->GetStride(), &Mesh->GetOffset());
		InContext.IASetIndexBuffer(Mesh->GetIndexBuffer(), Mesh->GetIndexFormat(), 0);

		for (size_t i = 0; i < PixelShaderResources.size(); ++i)
		{
			if (PixelShaderResources[i])
				InContext.PSSetShaderResource(PixelShaderResourceStartSlot + uint32_t(i), PixelShaderResources[i]->GetSRV());
		}

		InContext.DrawIndexed(Mesh->GetIndexCount());
	}
}
