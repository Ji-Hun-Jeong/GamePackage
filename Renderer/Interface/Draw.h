#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IPresenter
	{
		friend class RenderContext;
	public:
		IPresenter() {}
		virtual ~IPresenter() {}

	private:
		virtual void Present() const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXPresenter : public IPresenter
		{
		public:
			CDXPresenter(ComPtr<IDXGISwapChain> InSwapChain)
				: SwapChain(InSwapChain)
			{}
			~CDXPresenter()
			{}

		public:
			void Present() const override
			{
				SwapChain->Present(1, 0);
			}

		private:
			ComPtr<IDXGISwapChain> SwapChain;

		};
	}
}

