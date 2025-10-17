#include "pch.h"
#include "DXInfra.h"

namespace Graphics::DX
{
	CDXInfra::CDXInfra(HWND InWindowHandle, uint32_t InScreenWidth, uint32_t InScreenHeight)
		: DXResourceStorage(10000)
		, ScreenWidth(InScreenWidth)
		, ScreenHeight(InScreenHeight)
	{
		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE;
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_9_3 };
		D3D_FEATURE_LEVEL outputLevel;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Width = ScreenWidth;
		swapChainDesc.BufferDesc.Height = ScreenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = InWindowHandle;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		ComPtr<ID3D11Device> RawDevice;
		ComPtr<ID3D11DeviceContext> RawContext;
		ComPtr<IDXGISwapChain> RawSwapChain;
		HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, driverType, 0
			, createDeviceFlags, featureLevel, 1
			, D3D11_SDK_VERSION, &swapChainDesc
			, RawSwapChain.GetAddressOf(), RawDevice.GetAddressOf()
			, &outputLevel, RawContext.GetAddressOf());
		if (FAILED(hr)) assert(0);

		UINT NumOfMultiSamplingLevel;
		hr = RawDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4,
			&NumOfMultiSamplingLevel);
		if (FAILED(hr)) assert(0);

		Device = std::unique_ptr<CDXDevice>(new CDXDevice(RawDevice, DXResourceStorage));
		Context = std::unique_ptr<CDXContext>(new CDXContext(RawContext, DXResourceStorage));
		SwapChain = std::unique_ptr<CDXSwapChain>(new CDXSwapChain(RawSwapChain, NumOfMultiSamplingLevel, DXResourceStorage));
	}
}