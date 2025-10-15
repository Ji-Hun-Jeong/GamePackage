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

namespace Graphics
{
	class CRenderContext
	{
	public:
		CRenderContext() = default;
		virtual ~CRenderContext() = 0 {}

	public:
		virtual void OMSetRenderTarget(uint32_t InNumViews, const CRenderTargetView& InRenderTargetView, const CDepthStencilView* InDepthStencilView) = 0;
		virtual void ClearRenderTarget(const CRenderTargetView& InRenderTargetView, const float* InClearColor) = 0;
		virtual void IASetInputLayout(const CInputLayout& InInputLayout) = 0;
		virtual void IASetPrimitiveTopology(ETopology InTopology) = 0;
		virtual void IASetVertexBuffer(const CBuffer& InVertexBuffer, const uint32_t* InStride, const uint32_t* InOffset) = 0;
		virtual void IASetIndexBuffer(const CBuffer& InIndexBuffer, EGIFormat InFormat, uint32_t InOffset) = 0;
		virtual void VSSetShader(const CVertexShader& InVertexShader) = 0;
		virtual void VSSetConstantBuffers(uint32_t InStartSlot, uint32_t InNumBuffers, const CBuffer* InBuffers) = 0;
		virtual void RSSetViewPort(const TViewPort& InViewPort) = 0;
		virtual void RSSetState(const CRasterizerState& InRasterizerState) = 0;
		virtual void PSSetShader(const CPixelShader& InPixelShader) = 0;
		virtual void DrawIndexed(uint32_t InIndexCount) = 0;
		virtual void CopyBuffer(CBuffer& InBuffer, const TBufferMapResource& InBufferMapResource) = 0;

	private:

	};
}


