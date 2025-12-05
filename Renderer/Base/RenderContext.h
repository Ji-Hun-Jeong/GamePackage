#pragma once
#include "RHI/Buffer.h"
#include "RHI/VertexShader.h"
#include "RHI/InputLayout.h"
#include "RHI/PixelShader.h"
#include "RHI/Texture2D.h"
#include "RHI/RenderTargetView.h"
#include "RHI/RasterizerState.h"
#include "RHI/DepthStencilView.h"
#include "RHI/ViewPort.h"
#include "RHI/ShaderResourceView.h"
#include "RHI/SamplerState.h"
#include "RHI/BlendState.h"
#include "RHI/UnorderedAccessView.h"
#include "RHI/ComputeShader.h"

namespace Graphics
{
	class CRenderContext
	{
	public:
		CRenderContext() = default;
		virtual ~CRenderContext() = 0 {}

	public:
		virtual void OMSetRenderTargets(uint32_t InNumViews, const CRenderTargetView* InRenderTargetView, const CDepthStencilView* InDepthStencilView) = 0;
		virtual void ClearRenderTarget(const CRenderTargetView* InRenderTargetView, const float* InClearColor) = 0;
		virtual void IASetInputLayout(const CInputLayout* InInputLayout) = 0;
		virtual void IASetPrimitiveTopology(ETopology InTopology) = 0;
		virtual void IASetVertexBuffer(const CBuffer* InVertexBuffer, const uint32_t* InStride, const uint32_t* InOffset) = 0;
		virtual void IASetIndexBuffer(const CBuffer* InIndexBuffer, EGIFormat InFormat, uint32_t InOffset) = 0;
		virtual void VSSetShader(const CVertexShader* InVertexShader) = 0;
		virtual void VSSetConstantBuffers(uint32_t InStartSlot, uint32_t InNumBuffers, const std::vector<std::unique_ptr<CBuffer>>& InBuffers) = 0;
		virtual void VSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) = 0;
		virtual void VSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) = 0;
		virtual void RSSetViewPort(const TViewPort* InViewPort) = 0;
		virtual void RSSetState(const CRasterizerState* InRasterizerState) = 0;
		virtual void PSSetShader(const CPixelShader* InPixelShader) = 0;
		virtual void PSSetShaderResources(uint32_t InStartSlot, uint32_t InNumViews, const std::vector<std::unique_ptr<CShaderResourceView>>& InShaderResourceViews) = 0;
		virtual void PSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) = 0;
		virtual void PSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) = 0;
		virtual void DrawIndexed(uint32_t InIndexCount) = 0;
		virtual void DrawIndexedInstanced(uint32_t InIndexCountPerInstance, uint32_t InInstanceCount, uint32_t InStartIndexLocation, uint32_t InBaseVertexLocation
			, uint32_t InStartInstanceLocation) = 0;
		virtual void UpLoadBuffer(CBuffer& InBuffer, const void* InMapDataPoint, size_t InDataSize, EMapType InMapType) = 0;
		virtual void PSSetSampler(uint32_t InStartSlot, uint32_t InNumSamplers, const CSamplerState* InSamplers) = 0;
		virtual void OMSetBlendState(const CBlendState* InBlendState, const float* InBlendFactor, uint32_t InSampleMask) = 0;
		virtual void CopyResource(CBuffer& InDstBuffer, CBuffer& InSrcBuffer) = 0;
		virtual void DownLoadBuffer(void* InMappingPoint, size_t InByteWidth, const CBuffer& InBuffer, EMapType InMapType) = 0;
		virtual void CSSetShader(const CComputeShader* InComputeShader) = 0;
		virtual void CSSetUnorderedAccessView(uint32_t InStartSlot, const CUnorderedAccessView* InUAV) = 0;
		virtual void CSSetShaderResource(uint32_t InStartSlot, const CShaderResourceView* InShaderResourceView) = 0;
		virtual void CSSetConstantBuffer(uint32_t InStartSlot, const CBuffer* InBuffer) = 0;
		virtual void CSSetSampler(uint32_t InStartSlot, const CSamplerState* InSamplerState) = 0;
		virtual void Dispatch(uint32_t InThreadGroupCountX, uint32_t InThreadGroupCountY, uint32_t InThreadGroupCountZ) = 0;
	private:

	};
}


