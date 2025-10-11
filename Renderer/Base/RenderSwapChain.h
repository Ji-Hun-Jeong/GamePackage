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
		virtual const class CTexture2D* GetWindowTextureBuffer() const = 0;


	};
}