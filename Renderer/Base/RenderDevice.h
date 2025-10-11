#pragma once
#include "RenderContext.h"
#include "RenderSwapChain.h"

namespace Graphics
{
	class CRenderDevice
	{
	public:
		CRenderDevice() = default;
		virtual ~CRenderDevice() = 0 {}

	public:
		virtual std::unique_ptr<CPixelShader> CreatePixelShader(const std::wstring& InShaderPath) = 0;
		virtual std::pair<std::unique_ptr<CVertexShader>, std::unique_ptr<CInputLayout>> CreateVertexShaderAndInputLayout(const std::wstring& InShaderPath
			, const std::vector<TInputElementDesc>& InInputElementDescs) = 0;
		virtual std::unique_ptr<CBuffer> CreateBuffer(const TBufferDesc& InBufferDesc, TBufferInitalizeData* InBufferInitalizeData) = 0;
		virtual std::unique_ptr<CTexture2D> CreateTexture2D(const TTexture2DDesc& InTexture2DDesc, TBufferInitalizeData* InBufferInitalizeData) = 0;
		virtual std::unique_ptr<CRenderTargetView> CreateRenderTargetView(const CTexture2D& InTexture2D) = 0;
		virtual std::unique_ptr<CRasterizerState> CreateRasterizerState(const TRasterizerDesc& InRasterizerDesc) = 0;
		virtual std::unique_ptr<CDepthStencilView> CreateDepthStencilView(const CTexture2D& InTexture2D) = 0;

	};
}