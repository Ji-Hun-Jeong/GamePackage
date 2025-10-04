#pragma once
#include <Windows.h>

#include "Base/RenderDevice.h"
#include "DX.h"
#include "DXResourceStorage.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXDevice : public RenderDevice
	{
	public:
		CDXDevice(HWND InWindowHandle);
		~CDXDevice() = default;

	public:
		void CreateContextAndSwapChain(class CDXContext& InContext, class CDXSwapChain& InSwapChain);

	public:
		void CreatePixelShader(std::shared_ptr<CShader>& InPixelShader)
		{
			ComPtr<ID3DBlob> shaderBlob;
			ComPtr<ID3DBlob> errorBlob;

			UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
			const std::wstring& Wstr = InPixelShader->GetShaderPath();
			HRESULT hr = D3DCompileFromFile(
				Wstr.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
				"ps_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			if (FAILED(hr))	assert(0);

			ComPtr<ID3D11PixelShader> PixelShader;
			hr = Device->CreatePixelShader(shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(), NULL,
				PixelShader.GetAddressOf());
			if (FAILED(hr))	assert(0);

			InitalizeResource(InPixelShader.get(), PixelShader);
		}
		void CreateVertexShader(std::shared_ptr<CShader>& InShader, std::shared_ptr<CInputLayout>& InInputLayout)
		{
			ComPtr<ID3DBlob> shaderBlob;
			ComPtr<ID3DBlob> errorBlob;

			UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
			compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
			const std::wstring& WStr = InShader->GetShaderPath();

			HRESULT hr = D3DCompileFromFile(
				WStr.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
				"vs_5_0", compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			if (FAILED(hr))	assert(0);

			ComPtr<ID3D11VertexShader> VertexShader;
			hr = Device->CreateVertexShader(shaderBlob->GetBufferPointer(),
				shaderBlob->GetBufferSize(), NULL,
				VertexShader.GetAddressOf());
			if (FAILED(hr))	assert(0);

			std::vector<D3D11_INPUT_ELEMENT_DESC> BasicInputElements;
			auto& InputElementDescs = InInputLayout->GetInputElementDescs();
			for (auto IED : InputElementDescs)
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

			InitalizeResource(InShader.get(), VertexShader);
			InitalizeResource(InInputLayout.get(), InputLayout);
		}

	private:
		void InitalizeResource(IResource* InResource, ComPtr<ID3D11DeviceChild> InRawResource)
		{
			size_t ResourceHandle = DXResourceStorage.InsertResource(InRawResource);
			InResource->SetResourceHandle(ResourceHandle);
			InResource->SetReleaseResourceEvent([this](size_t InResourceHandle)
				{
					DXResourceStorage.EraseResource(InResourceHandle);
				});
		}

	private:
		ComPtr<ID3D11Device> Device;

		HWND WindowHandle;
		UINT ScreenWidth;
		UINT ScreenHeight;

		CDXResourceStorage DXResourceStorage;

	};
}