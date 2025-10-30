#include "pch.h"
#include "RenderStateObject.h"
#include <Renderer/Base/RenderContext.h>
#include "RenderResourceLoader.h"


void CRenderStateObject::MapBuffersOnUpdated(Graphics::CRenderContext& InContext)
{
	// UpdateConstBuffer
	for (auto& MappingInstance : VertexConstBufferMappingInstance)
	{
		if (MappingInstance == nullptr)
			continue;
		if (MappingInstance->bUpdated == false)
			continue;

		auto& CpuBuffer = MappingInstance.get()->CpuBuffer;
		auto& GpuBuffer = MappingInstance.get()->GpuBuffer;

		InContext.CopyBuffer(GpuBuffer.get(), CpuBuffer.data(), CpuBuffer.size());

		MappingInstance->bUpdated = false;
	}
}

void CRenderStateObject::BindRenderState(Graphics::CRenderContext& InContext)
{
	// SetConstBuffer
	for (size_t i = 0; i < VertexConstBufferMappingInstance.size(); ++i)
	{
		auto& MappingInstance = VertexConstBufferMappingInstance[i];
		if (MappingInstance == nullptr)
			continue;
		auto& GpuBuffer = MappingInstance->GpuBuffer;
		InContext.VSSetConstantBuffer(VertexConstBufferStartSlot + uint32_t(i), GpuBuffer.get());
	}

	if (PSO)
	{
		const float Factor = 1.0f;
		const float BlendFactor[4] = { Factor, Factor, Factor, 1.0f };
		InContext.OMSetBlendState(PSO->BlendState, BlendFactor, 0xffffffff);
		InContext.IASetPrimitiveTopology(PSO->PrimitiveTopology);
		InContext.IASetInputLayout(PSO->InputLayout);
		InContext.VSSetShader(PSO->VertexShader);
		InContext.RSSetState(PSO->RasterizerState);
		InContext.PSSetShader(PSO->PixelShader);
		InContext.PSSetSamplers(0, 1, PSO->SamplerState);
	}
	else
	{
		InContext.OMSetBlendState(nullptr, nullptr, 0xffffffff);
		InContext.IASetInputLayout(nullptr);
		InContext.VSSetShader(nullptr);
		InContext.RSSetState(nullptr);
		InContext.PSSetShader(nullptr);
		InContext.PSSetSamplers(0, 1, nullptr);
	}

	for (size_t i = 0; i < PixelShaderResources.size(); ++i)
	{
		if (PixelShaderResources[i])
			InContext.PSSetShaderResource(PixelShaderResourceStartSlot + uint32_t(i), &PixelShaderResources[i]->GetSRV());
		else
			InContext.PSSetShaderResource(PixelShaderResourceStartSlot + uint32_t(i), nullptr);
	}

	if (Mesh)
	{
		InContext.IASetVertexBuffer(&Mesh->GetVertexBuffer(), &Mesh->GetStride(), &Mesh->GetOffset());
		InContext.IASetIndexBuffer(&Mesh->GetIndexBuffer(), Mesh->GetIndexFormat(), 0);
		InContext.DrawIndexed(Mesh->GetIndexCount());
	}
	else
	{
		uint32_t Stride = 0;
		uint32_t Offset = 0;
		InContext.IASetVertexBuffer(nullptr, &Stride, &Offset);
		InContext.IASetIndexBuffer(nullptr, Graphics::EGIFormat(0), 0);
		InContext.DrawIndexed(0);
	}
}