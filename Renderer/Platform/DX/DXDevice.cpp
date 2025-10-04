#include "pch.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "DXSwapChain.h"

namespace Graphics::DX
{
	CDXDevice::CDXDevice(HWND _WindowHandle)
		: RenderDevice()
		, ScreenWidth(0)
		, ScreenHeight(0)
		, DXResourceStorage(10000)
	{
		RECT r = { 0 };
		::GetWindowRect(WindowHandle, &r);
		ScreenWidth = r.right - r.left;
		ScreenHeight = r.bottom - r.top;
	}
	void CDXDevice::CreateContextAndSwapChain(CDXContext& InContext, CDXSwapChain& InSwapChain)
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
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = WindowHandle;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		ComPtr<ID3D11DeviceContext> Context;
		ComPtr<IDXGISwapChain> SwapChain;
		HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, driverType, 0
			, createDeviceFlags, featureLevel, 1
			, D3D11_SDK_VERSION, &swapChainDesc
			, SwapChain.GetAddressOf(), Device.GetAddressOf()
			, &outputLevel, Context.GetAddressOf());
		if (FAILED(hr)) assert(0);

		UINT NumOfMultiSamplingLevel;
		hr = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
			&NumOfMultiSamplingLevel);
		if (FAILED(hr)) assert(0);

		InContext.Initalize(Context);
		InSwapChain.Initalize(SwapChain, NumOfMultiSamplingLevel);
	}
}
