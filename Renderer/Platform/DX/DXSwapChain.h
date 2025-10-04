#pragma once
#include "Base/SwapChain.h"
#include "DX.h"

namespace Graphics::DX
{
	using Microsoft::WRL::ComPtr;
	class CDXSwapChain : public RenderSwapChain
	{
	public:
		CDXSwapChain()
			: SwapChain(nullptr)
		{}
		~CDXSwapChain() = default;

	public:
		void Initalize(ComPtr<IDXGISwapChain> InSwapChain, UINT InNumOfMultiSamplingLevel)
		{
			SwapChain = InSwapChain;
			NumOfMultiSamplingLevel = InNumOfMultiSamplingLevel;
		}

	private:
		ComPtr<IDXGISwapChain> SwapChain;
		UINT NumOfMultiSamplingLevel;
	};
}


