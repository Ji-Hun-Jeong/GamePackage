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
		void VSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) override
		{
			ID3D11ShaderResourceView* ShaderResourceView = nullptr;
			if (InShaderResourceView)
				ShaderResourceView = DXResourceStorage.GetResource<ID3D11ShaderResourceView>(InShaderResourceView->GetResourceHandle());
			Context->VSSetShaderResources(InStartSlot, 1, &ShaderResourceView);
		}
		void RSSetViewPort(const TViewPort* InViewPort) override;
		void RSSetState(const CRasterizerState* InRasterizerState) override;
		void PSSetShader(const CPixelShader* InPixelShader) override;
		void PSSetShaderResources(uint32_t InStartSlot, uint32_t InNumViews, const std::vector<std::unique_ptr<CShaderResourceView>>& InShaderResourceViews) override;
		void PSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) override;
		void PSSetSampler(uint32_t InStartSlot, uint32_t InNumSamplers, const CSamplerState* InSamplers) override;
		void PSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) override;
		void OMSetBlendState(const CBlendState* InBlendState, const float* InBlendFactor, uint32_t InSampleMask) override;
		void DrawIndexed(uint32_t InIndexCount) override
		{
			Context->DrawIndexed(InIndexCount, 0, 0);
		}
		void DrawIndexedInstanced(uint32_t InIndexCountPerInstance, uint32_t InInstanceCount, uint32_t InStartIndexLocation, uint32_t InBaseVertexLocation
			, uint32_t InStartInstanceLocation) override
		{
			Context->DrawIndexedInstanced(InIndexCountPerInstance, InInstanceCount, InStartIndexLocation, InBaseVertexLocation, InStartInstanceLocation);
		}
		void UpLoadBuffer(CBuffer& InBuffer, const void* InMapDataPoint, size_t InDataSize, EMapType InMapType) override;
		void DownLoadBuffer(void* InMappingPoint, size_t InByteWidth, const CBuffer& InBuffer, EMapType InMapType) override;
		void CopyResource(CBuffer& InDstBuffer, CBuffer& InSrcBuffer) override;
		void CSSetShader(const CComputeShader* InComputeShader) override
		{
			ID3D11ComputeShader* RawComputeShader = nullptr;
			if (InComputeShader)
				RawComputeShader = DXResourceStorage.GetResource<ID3D11ComputeShader>(InComputeShader->GetResourceHandle());
			Context->CSSetShader(RawComputeShader, nullptr, 0);
		}
		void CSSetUnorderedAccessView(uint32_t InStartSlot, const CUnorderedAccessView* InUAV) override
		{
			ID3D11UnorderedAccessView* RawUAV = nullptr;
			if(InUAV)
				RawUAV = DXResourceStorage.GetResource<ID3D11UnorderedAccessView>(InUAV->GetResourceHandle());
			Context->CSSetUnorderedAccessViews(InStartSlot, 1, &RawUAV, nullptr);
		}
		void CSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) override
		{
			ID3D11ShaderResourceView* RawSRV = nullptr;
			if(InShaderResourceView)
				RawSRV = DXResourceStorage.GetResource<ID3D11ShaderResourceView>(InShaderResourceView->GetResourceHandle());
			Context->CSSetShaderResources(InStartSlot, 1, &RawSRV);
		}
		void CSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) override
		{
			ID3D11Buffer* RawBuffer = nullptr;
			if(InBuffer)
				RawBuffer = DXResourceStorage.GetResource<ID3D11Buffer>(InBuffer->GetResourceHandle());
			Context->CSSetConstantBuffers(InStartSlot, 1, &RawBuffer);
		}
		void CSSetSampler(uint32_t InStartSlot, const CSamplerState* InSamplerState) override
		{
			ID3D11SamplerState* RawSamplerState = nullptr;
			if(InSamplerState)
				RawSamplerState = DXResourceStorage.GetResource<ID3D11SamplerState>(InSamplerState->GetResourceHandle());
			Context->CSSetSamplers(InStartSlot, 1, &RawSamplerState);
		}
		void Dispatch(uint32_t InThreadGroupCountX, uint32_t InThreadGroupCountY, uint32_t InThreadGroupCountZ) override
		{
			Context->Dispatch(InThreadGroupCountX, InThreadGroupCountY, InThreadGroupCountZ);
			Context->Flush();
		}

	private:
		ComPtr<ID3D11DeviceContext> Context;
		CDXResourceStorage& DXResourceStorage;

	};
}


