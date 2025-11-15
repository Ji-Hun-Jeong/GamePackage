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
		CDXInfra(HWND InWindowHandle, uint32_t InScreenWidth, uint32_t InScreenHeight);
		~CDXInfra();

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

