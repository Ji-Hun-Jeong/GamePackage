#pragma once

namespace Graphics
{
	class CRenderSwapChain
	{
	public:
		CRenderSwapChain() = default;
		virtual ~CRenderSwapChain() = default;

	public:
		virtual void Present() = 0;
		virtual void ResizeBuffers(uint32_t InBufferCount, uint32_t InScreenWidth, uint32_t InScreenHeight, EGIFormat InFormat) = 0;
		virtual void GenerateWindowTextureBuffer() = 0;
		virtual const class CTexture2D* GetWindowTextureBuffer() const = 0;


	};
}