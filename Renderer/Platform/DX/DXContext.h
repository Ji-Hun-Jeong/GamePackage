#pragma once
#include "Base/RenderContext.h"
#include "DX.h"
#include "DXResourceStorage.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXContext : public CRenderContext
	{
		friend class CDXInfra;
		CDXContext(ComPtr<ID3D11DeviceContext> InContext, CDXResourceStorage& InDXResourceStorage)
			: CRenderContext()
			, Context(InContext)
			, DXResourceStorage(InDXResourceStorage)
		{}
	public:
		~CDXContext() = default;

	public:
		void OMSetRenderTargets(uint32_t InNumViews, const CRenderTargetView* InRenderTargetView, const CDepthStencilView* InDepthStencilView) override;
		void ClearRenderTarget(const CRenderTargetView* InRenderTargetView, const float* InClearColor) override;
		void IASetInputLayout(const CInputLayout* InInputLayout) override;
		void IASetPrimitiveTopology(ETopology InTopology) override;
		void IASetVertexBuffer(const CBuffer* InVertexBuffer, const uint32_t* InStride, const uint32_t* InOffset) override;
		void IASetIndexBuffer(const CBuffer* InIndexBuffer, EGIFormat InFormat, uint32_t InOffset) override;
		void VSSetShader(const CVertexShader* InVertexShader) override;
		void VSSetConstantBuffers(uint32_t InStartSlot, uint32_t InNumBuffers, const std::vector<std::unique_ptr<CBuffer>>& InBuffers) override;
		void VSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) override;
		void RSSetViewPort(const TViewPort* InViewPort) override;
		void RSSetState(const CRasterizerState* InRasterizerState) override;
		void PSSetShader(const CPixelShader* InPixelShader) override;
		void PSSetShaderResources(uint32_t InStartSlot, uint32_t InNumViews, const std::vector<std::unique_ptr<CShaderResourceView>>& InShaderResourceViews) override;
		void PSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) override;
		void PSSetSamplers(uint32_t InStartSlot, uint32_t InNumSamplers, const CSamplerState* InSamplers) override;
		void PSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) override;
		void OMSetBlendState(const CBlendState* InBlendState, const float* InBlendFactor, uint32_t InSampleMask) override;
		void DrawIndexed(uint32_t InIndexCount) override
		{
			Context->DrawIndexed(InIndexCount, 0, 0);
		}
		void CopyBuffer(CBuffer* InBuffer, const void* InMapDataPoint, size_t InDataSize) override
		{
			if (InBuffer == nullptr)
				return;
			const TBufferDesc& BufferDesc = InBuffer->GetBufferDesc();

			ID3D11Resource* Resource = DXResourceStorage.GetResource<ID3D11Resource>(InBuffer->GetResourceHandle());
			D3D11_MAPPED_SUBRESOURCE MappedSubResource;
			ZeroMemory(&MappedSubResource, sizeof(MappedSubResource));
			Context->Map(Resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);
			memcpy(MappedSubResource.pData, InMapDataPoint, InDataSize);
			Context->Unmap(Resource, 0);
		}

	private:
		ComPtr<ID3D11DeviceContext> Context;
		CDXResourceStorage& DXResourceStorage;

	};
}


