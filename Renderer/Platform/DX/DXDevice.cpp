#include "pch.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "DXSwapChain.h"

namespace Graphics::DX
{
	CDXDevice::CDXDevice(ComPtr<ID3D11Device> InDevice, CDXResourceStorage& InDXResourceStorage)
		: CRenderDevice()
		, Device(InDevice)
		, DXResourceStorage(InDXResourceStorage)
	{

	}

	CDXDevice::~CDXDevice()
	{

	}

	std::unique_ptr<CPixelShader> CDXDevice::CreatePixelShader(const std::wstring& InShaderPath)
	{
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		HRESULT hr = D3DCompileFromFile(
			InShaderPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
			"ps_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		if (FAILED(hr))	assert(0);

		ComPtr<ID3D11PixelShader> PixelShader;
		hr = Device->CreatePixelShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), NULL,
			PixelShader.GetAddressOf());
		if (FAILED(hr))	assert(0);

		size_t ResourceHandle = DXResourceStorage.InsertResource(PixelShader);

		return std::make_unique<CPixelShader>(ResourceHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1), InShaderPath);
	}

	std::pair<std::unique_ptr<CVertexShader>, std::unique_ptr<CInputLayout>> CDXDevice::CreateVertexShaderAndInputLayout(const std::wstring& InShaderPath, const std::vector<TInputElementDesc>& InInputElementDescs)
	{
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = D3DCompileFromFile(
			InShaderPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
			"vs_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		if (FAILED(hr))	assert(0);

		ComPtr<ID3D11VertexShader> VertexShader;
		hr = Device->CreateVertexShader(shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), NULL,
			VertexShader.GetAddressOf());
		if (FAILED(hr))	assert(0);

		std::vector<D3D11_INPUT_ELEMENT_DESC> BasicInputElements;
		for (auto IED : InInputElementDescs)
		{
			D3D11_INPUT_ELEMENT_DESC Desc;
			ZeroMemory(&Desc, sizeof(Desc));
			switch (IED.SementicName)
			{
			case ESementicName::Position:
				Desc.SemanticName = "POSITION";
				break;
			case ESementicName::Color:
				Desc.SemanticName = "COLOR";
				break;
			case ESementicName::UV:
				Desc.SemanticName = "TEXCOORD";
				break;
			default:
				assert(0);
			}
			switch (IED.Format)
			{
			case EFormat::Vector3:
				Desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case EFormat::Vector2:
				Desc.Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			default:
				assert(0);
			}
			Desc.AlignedByteOffset = IED.Offset;
			switch (IED.InputClass)
			{
			case EInputClass::VertexData:
				Desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				break;
			default:
				assert(0);
			}
			BasicInputElements.push_back(Desc);
		}

		ComPtr<ID3D11InputLayout> InputLayout;
		hr = Device->CreateInputLayout(BasicInputElements.data(), UINT(BasicInputElements.size()),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(), InputLayout.GetAddressOf());
		if (FAILED(hr))	assert(0);

		size_t VertexShaderHandle = DXResourceStorage.InsertResource(VertexShader);
		size_t InputLayoutHandle = DXResourceStorage.InsertResource(InputLayout);

		return std::make_pair(
			std::make_unique<CVertexShader>(VertexShaderHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1), InShaderPath)
			, std::make_unique<CInputLayout>(InputLayoutHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1), InInputElementDescs)
		);
	}

	std::unique_ptr<CBuffer> CDXDevice::CreateBuffer(const TBufferDesc& InBufferDesc, const TBufferInitalizeData* InBufferInitalizeData)
	{
		if (InBufferDesc.Usage == EUsage::UsageImmutable && InBufferInitalizeData == nullptr)
			assert(0);

		D3D11_BUFFER_DESC D3DBufferDesc;
		ZeroMemory(&D3DBufferDesc, sizeof(D3DBufferDesc));
		memcpy(&D3DBufferDesc, &InBufferDesc, sizeof(TBufferDesc));

		ComPtr<ID3D11Buffer> Buffer;
		if (InBufferInitalizeData)
		{
			// 딱 데이터를 한번만 전달할 때 사용
			// map을 사용할 때는 필요없음
			D3D11_SUBRESOURCE_DATA BufferData = { 0 };
			ZeroMemory(&BufferData, sizeof(BufferData));
			memcpy(&BufferData, InBufferInitalizeData, sizeof(TBufferInitalizeData));
			// default + initdata
			HRESULT hr = Device->CreateBuffer(&D3DBufferDesc, &BufferData, Buffer.GetAddressOf());
			if (FAILED(hr)) assert(0);
		}
		else
		{
			// dynamic + map
			HRESULT hr = Device->CreateBuffer(&D3DBufferDesc, nullptr, Buffer.GetAddressOf());
			if (FAILED(hr)) assert(0);
		}

		size_t BufferHandle = DXResourceStorage.InsertResource(Buffer);
		return std::make_unique<CBuffer>(BufferHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1), InBufferDesc, InBufferInitalizeData);
	}

	std::unique_ptr<CTexture2D> CDXDevice::CreateTexture2D(const TTexture2DDesc& InTexture2DDesc, const TBufferInitalizeData* InBufferInitalizeData)
	{
		D3D11_TEXTURE2D_DESC D3DTexture2DDesc;
		memcpy(&D3DTexture2DDesc, &InTexture2DDesc, sizeof(TTexture2DDesc));

		D3D11_SUBRESOURCE_DATA D3DTexutreInitData = { 0 };
		if (InBufferInitalizeData)
			memcpy(&D3DTexutreInitData, InBufferInitalizeData, sizeof(TBufferInitalizeData));

		ComPtr<ID3D11Texture2D> Texture2D;
		Device->CreateTexture2D(&D3DTexture2DDesc, &D3DTexutreInitData, Texture2D.GetAddressOf());

		size_t Texture2DHandle = DXResourceStorage.InsertResource(Texture2D);

		return std::make_unique<CTexture2D>(Texture2DHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1));
	}

	std::unique_ptr<CRenderTargetView> CDXDevice::CreateRenderTargetView(const CTexture2D& InTexture2D)
	{
		ComPtr<ID3D11Texture2D> TextureBuffer = DXResourceStorage.GetResource<ID3D11Texture2D>(InTexture2D.GetResourceHandle());
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		Device->CreateRenderTargetView(TextureBuffer.Get(), nullptr, RenderTargetView.GetAddressOf());

		size_t RenderTargetViewHandle = DXResourceStorage.InsertResource(RenderTargetView);

		return std::make_unique<CRenderTargetView>(RenderTargetViewHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1));
	}

	std::unique_ptr<CRasterizerState> CDXDevice::CreateRasterizerState(const TRasterizerDesc& InRasterizerDesc)
	{
		D3D11_RASTERIZER_DESC RDesc;
		memcpy(&RDesc, &InRasterizerDesc, sizeof(TRasterizerDesc));

		ComPtr<ID3D11RasterizerState> RasterizerState;
		HRESULT hr = Device->CreateRasterizerState(&RDesc, RasterizerState.GetAddressOf());
		if (FAILED(hr)) assert(0);

		size_t RasterizerStateHandle = DXResourceStorage.InsertResource(RasterizerState);
		return std::make_unique<CRasterizerState>(RasterizerStateHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1), InRasterizerDesc);
	}

	std::unique_ptr<CDepthStencilView> CDXDevice::CreateDepthStencilView(const CTexture2D& InTexture2D)
	{
		ComPtr<ID3D11Texture2D> DepthBuffer = DXResourceStorage.GetResource<ID3D11Texture2D>(InTexture2D.GetResourceHandle());

		ComPtr<ID3D11DepthStencilView> DepthStencilView;
		Device->CreateDepthStencilView(DepthBuffer.Get(), nullptr, DepthStencilView.GetAddressOf());

		size_t DepthStencilViewHandle = DXResourceStorage.InsertResource(DepthStencilView);

		return std::make_unique<CDepthStencilView>(DepthStencilViewHandle, std::bind(&CDXDevice::ReleaseResource, this, std::placeholders::_1));
	}
}
