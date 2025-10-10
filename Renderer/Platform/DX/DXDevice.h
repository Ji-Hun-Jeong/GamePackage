#pragma once
#include <Windows.h>

#include "Base/RenderDevice.h"
#include "DX.h"
#include "DXResourceStorage.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXDevice : public CRenderDevice
	{
	public:
		CDXDevice(HWND InWindowHandle);
		~CDXDevice();

	public:
		// unique_ptr전방선언할 때는 네임스페이스까지 다 써서 명시해주기
		// 지금은 가상함수를 구현하는거라서 RenderDevice쪽에 선언되어있음
		TDeviceInitData CreateContextAndSwapChain() override;

	public:
		std::unique_ptr<CPixelShader> CreatePixelShader(const std::wstring& InShaderPath);
		std::pair<std::unique_ptr<CVertexShader>, std::unique_ptr<CInputLayout>> CreateVertexShaderAndInputLayout(const std::wstring& InShaderPath
			, const std::vector<TInputElementDesc>& InInputElementDescs);
		std::unique_ptr<CBuffer> CreateBuffer(const TBufferDesc& InBufferDesc, TBufferInitalizeData* InBufferInitalizeData);
		std::unique_ptr<CTexture2D> CreateTexture2D(const TTexture2DDesc& InTexture2DDesc, TBufferInitalizeData* InBufferInitalizeData);
		std::unique_ptr<CRenderTargetView> CreateRenderTargetView(const CTexture2D& InTexture2D);
		std::unique_ptr<CRasterizerState> CreateRasterizerState(const TRasterizerDesc& InRasterizerDesc);
		std::unique_ptr<CDepthStencilView> CreateDepthStencilView(const CTexture2D& InTexture2D);


	private:
		void ReleaseResource(size_t InResourceHandle)
		{
			DXResourceStorage.VacateResource(InResourceHandle);
		}

	private:
		CDXResourceStorage DXResourceStorage;

		ComPtr<ID3D11Device> Device;

		HWND WindowHandle;
		UINT ScreenWidth;
		UINT ScreenHeight;

		std::unique_ptr<CRenderContext> Context;
		std::unique_ptr<CRenderSwapChain> SwapChain;

	};
}