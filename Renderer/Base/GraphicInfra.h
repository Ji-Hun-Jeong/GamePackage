#pragma once
#include "RenderDevice.h"
#include "RenderContext.h"
#include "RenderSwapChain.h"

namespace Graphics
{
	class IGraphicInfra
	{
	public:
		IGraphicInfra()
		{}
		virtual ~IGraphicInfra() {}

	public:
		virtual CRenderDevice& GetDevice() = 0;
		virtual CRenderContext& GetContext() = 0;
		virtual CRenderSwapChain& GetSwapChain() = 0;

	};
}


