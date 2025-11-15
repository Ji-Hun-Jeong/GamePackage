#pragma once
#include "RenderDevice.h"
#include "RenderContext.h"
#include "RenderSwapChain.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

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


