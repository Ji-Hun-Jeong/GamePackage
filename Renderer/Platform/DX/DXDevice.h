#pragma once
#include <Windows.h>

#include "Base/RenderDevice.h"
#include "DX.h"

namespace Graphics
{
	using Microsoft::WRL::ComPtr;
	class CDXDevice : public RenderDevice
	{
	public:
		CDXDevice(HWND _WindowHandle);

	public:
		IRenderTargetView* GenerateMainRenderTargetView() override;

		IViewPort* GenerateMainViewPort() override;

		IMesh* GenerateMesh(void* _VertexData, size_t _VertexSize, size_t _NumOfVertex
			, void* _IndexData, size_t _IndexSize, size_t _NumOfIndex) override;

		IConstBuffer* GenerateConstBuffer(const std::vector<CpuConstData>& InCpuData) override;

		IVertexShader* GenerateVertexShaderAndInputLayout(const std::string& InPath
			, const std::vector<InputElementDesc>& InInputElements) override;

		IPixelShader* GeneratePixelShader(const std::string& InPath) override;

		ISampler* GenerateSampler() override;

		IRasterizerState* GenerateSolidCWState() override;

		IDepthStencilState* GenerateBasicDepthStencilState() override;

		ITopology* GenerateTopology(eTopology InTopology) override;

		IPresenter* GeneratePresenter() override;

		IShaderResource* GenerateShaderResource(const std::vector<std::string>& InPaths) override;

		IBlendState* GenerateBlendState(const float* InBlendFactor) override;

		IComputeShader* GenerateComputeShader(const std::string& InPath) override;

	private:
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> Context;
		ComPtr<IDXGISwapChain> SwapChain;
		UINT NumOfMultiSamplingLevel;

		HWND WindowHandle;
		UINT ScreenWidth;
		UINT ScreenHeight;

	};
}