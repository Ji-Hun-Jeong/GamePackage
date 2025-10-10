#pragma once

namespace Graphics
{
	class CRenderSwapChain
	{
	public:
		CRenderSwapChain(std::unique_ptr<class CTexture2D> InWindowTextureBuffer)
			: WindowTextureBuffer(std::move(InWindowTextureBuffer))
		{
		}
		virtual ~CRenderSwapChain() = default;

	public:
		virtual void Present() = 0;
		const class CTexture2D* GetWindowTextureBuffer() const { return WindowTextureBuffer.get(); }

	private:
		std::unique_ptr<class CTexture2D> WindowTextureBuffer;

	};
}