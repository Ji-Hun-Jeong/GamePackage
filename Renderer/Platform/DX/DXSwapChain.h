#pragma once
#include "Base/RenderSwapChain.h"
#include "DX.h"
#include "DXResourceStorage.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXSwapChain : public CRenderSwapChain
	{
		friend class CDXInfra;
		CDXSwapChain(ComPtr<IDXGISwapChain> InSwapChain, UINT InNumOfMultiSamplingLevel, CDXResourceStorage& InDXResourceStorage);
	public:
		~CDXSwapChain() 
		{
			
		}

	public:
		const CTexture2D* GetWindowTextureBuffer() const override
		{
			return WindowTexture.get();
		}
		void Present() override
		{
			SwapChain->Present(1, 0);
		}

	private:
		ComPtr<IDXGISwapChain> SwapChain;
		UINT NumOfMultiSamplingLevel;

		CDXResourceStorage& DXResourceStorage;

		std::unique_ptr<CTexture2D> WindowTexture;
	};
}


