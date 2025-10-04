#include "pch.h"
#include "DXDevice.h"

namespace Graphics
{
	CDXDevice::CDXDevice(HWND _WindowHandle)
		: RenderDevice()
		, NumOfMultiSamplingLevel(0)
		, ScreenWidth(0)
		, ScreenHeight(0)
	{
		RECT r = { 0 };
		::GetWindowRect(WindowHandle, &r);
		ScreenWidth = r.right - r.left;
		ScreenHeight = r.bottom - r.top;

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

		HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, driverType, 0
			, createDeviceFlags, featureLevel, 1
			, D3D11_SDK_VERSION, &swapChainDesc
			, SwapChain.GetAddressOf(), Device.GetAddressOf()
			, &outputLevel, Context.GetAddressOf());
		if (FAILED(hr)) assert(0);


		hr = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
			&NumOfMultiSamplingLevel);
		if (FAILED(hr)) assert(0);
	}
	IRenderTargetView* CDXDevice::GenerateMainRenderTargetView()
	{
		ComPtr<ID3D11Texture2D> Buffer;
		HRESULT hr = SwapChain->GetBuffer(0, IID_PPV_ARGS(Buffer.GetAddressOf()));
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		if (Buffer)
			hr = Device->CreateRenderTargetView(Buffer.Get(), nullptr, RenderTargetView.GetAddressOf());
		if (FAILED(hr)) assert(0);

		D3D11_TEXTURE2D_DESC DDesc;
		ZeroMemory(&DDesc, sizeof(DDesc));
		DDesc.Width = ScreenWidth;
		DDesc.Height = ScreenHeight;
		DDesc.MipLevels = 1;
		DDesc.ArraySize = 1;
		DDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DDesc.SampleDesc.Count = 1; // how many multisamples
		DDesc.SampleDesc.Quality = 0;
		DDesc.Usage = D3D11_USAGE_DEFAULT;
		DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DDesc.CPUAccessFlags = 0;
		DDesc.MiscFlags = 0;

		hr = Device->CreateTexture2D(&DDesc, nullptr, Buffer.GetAddressOf());
		ComPtr<ID3D11DepthStencilView> DepthStencilView;
		if (FAILED(hr)) assert(0);

		hr = Device->CreateDepthStencilView(Buffer.Get(), nullptr, DepthStencilView.GetAddressOf());
		if (FAILED(hr)) assert(0);

		std::vector<ComPtr<ID3D11RenderTargetView>> RenderTargetViews{ RenderTargetView };

		return new DX::CDXRenderTargetView(Context, std::move(RenderTargetViews), DepthStencilView);
	}
	IViewPort* CDXDevice::GenerateMainViewPort()
	{
		D3D11_VIEWPORT ViewPort;
		ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		ViewPort.Width = static_cast<float>(ScreenWidth);
		ViewPort.Height = static_cast<float>(ScreenHeight);
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		std::vector<D3D11_VIEWPORT> ViewPorts{ ViewPort };
		
		return new DX::CDXViewPort(Context, std::move(ViewPorts));
	}
	IMesh* CDXDevice::GenerateMesh(void* _VertexData, size_t _VertexSize, size_t _NumOfVertex, void* _IndexData, size_t _IndexSize, size_t _NumOfIndex)
	{
		return nullptr;
	}
	IConstBuffer* CDXDevice::GenerateConstBuffer(const std::vector<CpuConstData>& InCpuData)
	{
		std::vector<ComPtr<ID3D11Buffer>> Buffers;

		Buffers.reserve(InCpuData.size());
		for (size_t i = 0; i < InCpuData.size(); ++i)
		{
			assert(InCpuData[i].Size % 16 == 0);
			D3D11_BUFFER_DESC BufferDesc;
			ZeroMemory(&BufferDesc, sizeof(BufferDesc));
			BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			BufferDesc.ByteWidth = InCpuData[i].Size;
			BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			BufferDesc.StructureByteStride = 0;

			ComPtr<ID3D11Buffer> Buffer;
			HRESULT hr = Device->CreateBuffer(&BufferDesc, nullptr, Buffer.GetAddressOf());
			if (FAILED(hr)) assert(0);
			Buffers.push_back(Buffer);
		}
		return nullptr;
		//return new DX::CDXConstBuffer(Context, std::move(Buffers), InCpuData);
	}
	IVertexShader* CDXDevice::GenerateVertexShaderAndInputLayout(const std::string& InPath, const std::vector<InputElementDesc>& InInputElements)
	{
		return nullptr;
	}
	IPixelShader* CDXDevice::GeneratePixelShader(const std::string& InPath)
	{
		return nullptr;
	}
	ISampler* CDXDevice::GenerateSampler()
	{
		return nullptr;
	}
	IRasterizerState* CDXDevice::GenerateSolidCWState()
	{
		return nullptr;
	}
	IDepthStencilState* CDXDevice::GenerateBasicDepthStencilState()
	{
		return nullptr;
	}
	ITopology* CDXDevice::GenerateTopology(eTopology InTopology)
	{
		return nullptr;
	}
	IPresenter* CDXDevice::GeneratePresenter()
	{
		return nullptr;
	}
	IShaderResource* CDXDevice::GenerateShaderResource(const std::vector<std::string>& InPaths)
	{
		return nullptr;
	}
	IBlendState* CDXDevice::GenerateBlendState(const float* InBlendFactor)
	{
		return nullptr;
	}
	IComputeShader* CDXDevice::GenerateComputeShader(const std::string& InPath)
	{
		return nullptr;
	}
}
