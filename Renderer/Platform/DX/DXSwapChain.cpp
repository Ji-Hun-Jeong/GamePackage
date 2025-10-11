#include "pch.h"
#include "DXSwapChain.h"
#include "RHI/Texture2D.h"

namespace Graphics::DX
{
	CDXSwapChain::CDXSwapChain(ComPtr<IDXGISwapChain> InSwapChain, UINT InNumOfMultiSamplingLevel, CDXResourceStorage& InDXResourceStorage)
		: SwapChain(InSwapChain)
		, NumOfMultiSamplingLevel(InNumOfMultiSamplingLevel)
		, DXResourceStorage(InDXResourceStorage)
		, WindowTexture(nullptr)
	{
		ComPtr<ID3D11Texture2D> WindowTextureBuffer;
		HRESULT hr = SwapChain->GetBuffer(0, IID_PPV_ARGS(WindowTextureBuffer.GetAddressOf()));
		if (FAILED(hr))	assert(0);

		size_t WindowBufferHandle = DXResourceStorage.InsertResource(WindowTextureBuffer);
		WindowTexture = std::make_unique<CTexture2D>(WindowBufferHandle, [this](size_t InRHIHandle)->void
			{
				DXResourceStorage.VacateResource(InRHIHandle);
			});
	}
}