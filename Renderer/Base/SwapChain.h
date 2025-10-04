#pragma once

namespace Graphics
{
	class RenderSwapChain
	{
	public:
		RenderSwapChain() = default;
		virtual ~RenderSwapChain() = default;

	public:
		virtual void Present() = 0;

	private:

	};
}