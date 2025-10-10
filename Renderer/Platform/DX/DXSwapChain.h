#pragma once
#include "Base/RenderSwapChain.h"
#include "DX.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXSwapChain : public CRenderSwapChain
	{
	public:
		CDXSwapChain(std::unique_ptr<class CTexture2D> InWindowTextureBuffer, ComPtr<IDXGISwapChain> InSwapChain, UINT InNumOfMultiSamplingLevel)
			: CRenderSwapChain(std::move(InWindowTextureBuffer))
			, SwapChain(InSwapChain)
			, NumOfMultiSamplingLevel(InNumOfMultiSamplingLevel)
		{}
		~CDXSwapChain() 
		{
			int a = 1;
		}

	public:
		void Present() override
		{
			SwapChain->Present(1, 0);
		}

	private:
		ComPtr<IDXGISwapChain> SwapChain;
		UINT NumOfMultiSamplingLevel;

	};
}


