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
		friend class CDXInfra;
		CDXDevice(ComPtr<ID3D11Device> InDevice, CDXResourceStorage& InDXResourceStorage);
	public:
		~CDXDevice();

	public:
		// unique_ptr전방선언할 때는 네임스페이스까지 다 써서 명시해주기
		// 지금은 가상함수를 구현하는거라서 RenderDevice쪽에 선언되어있음

	public:
		std::unique_ptr<CPixelShader> CreatePixelShader(const std::wstring& InShaderPath) override;
		std::pair<std::unique_ptr<CVertexShader>, std::unique_ptr<CInputLayout>> CreateVertexShaderAndInputLayout(const std::wstring& InShaderPath
			, const std::vector<TInputElementDesc>& InInputElementDescs) override;
		std::unique_ptr<CBuffer> CreateBuffer(const TBufferDesc& InBufferDesc, const TBufferInitalizeData* InBufferInitalizeData) override;
		std::unique_ptr<CTexture2D> CreateTexture2D(const TTexture2DDesc& InTexture2DDesc, const TBufferInitalizeData* InBufferInitalizeData);
		std::unique_ptr<CRenderTargetView> CreateRenderTargetView(const CTexture2D& InTexture2D) override;
		std::unique_ptr<CRasterizerState> CreateRasterizerState(const TRasterizerDesc& InRasterizerDesc) override;
		std::unique_ptr<CDepthStencilView> CreateDepthStencilView(const CTexture2D& InTexture2D) override;
		std::unique_ptr<CShaderResourceView> CreateImage(const std::wstring& InImagePath) override;
		std::unique_ptr<CSamplerState> CreateSamplerState(const TSamplerDesc& InSamplerDesc) override;

	private:
		void ReleaseResource(size_t InRHIHandle)
		{
			DXResourceStorage.VacateResource(InRHIHandle);
		}

	private:
		CDXResourceStorage& DXResourceStorage;

		ComPtr<ID3D11Device> Device;

	};
}