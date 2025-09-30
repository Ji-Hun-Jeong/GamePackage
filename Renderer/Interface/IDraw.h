#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IPresenter
	{
	public:
		IPresenter() {}
		virtual ~IPresenter() {}

	public:
		virtual void Present() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXPresenter : public IPresenter
		{
		public:
			CDXPresenter(ComPtr<ID3D11DeviceContext>& _Context, ComPtr<IDXGISwapChain>& _SwapChain)
				: Context(_Context)
				, SwapChain(_SwapChain)
			{}
			~CDXPresenter()
			{}

		public:
			void Present() const override
			{
				SwapChain->Present(1, 0);
			}

		private:
			ComPtr<ID3D11DeviceContext>& Context;
			ComPtr<IDXGISwapChain>& SwapChain;

		};
	}
}

