#pragma once
#include "Base/RenderDevice.h"
#include "DX.h"

namespace Graphics
{
	using Microsoft::WRL::ComPtr;
	class CDXDevice : public IRenderDevice
	{
	public:
		CDXDevice(HWND _WindowHandle);


	private:
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> Context;
		ComPtr<IDXGISwapChain> SwapChain;
		UINT NumOfMultiSamplingLevel;

	};
}