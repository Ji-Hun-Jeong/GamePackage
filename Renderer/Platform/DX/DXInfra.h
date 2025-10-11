#pragma once
#include "Base/GraphicInfra.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "DXSwapChain.h"

namespace Graphics::DX
{
	class CDXInfra : public IGraphicInfra
	{
	public:
		CDXInfra(HWND InWindowHandle);
		~CDXInfra() = default;

	public:
		CRenderDevice& GetDevice() override { return *Device.get(); }
		CRenderContext& GetContext() override { return *Context.get(); }
		CRenderSwapChain& GetSwapChain() override { return *SwapChain.get(); }

	private:
		CDXResourceStorage DXResourceStorage;

		std::unique_ptr<CDXDevice> Device;
		std::unique_ptr<CDXContext> Context;
		std::unique_ptr<CDXSwapChain> SwapChain;

	};
}

