#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IRenderTargetView
	{
	public:
		IRenderTargetView() = default;
		virtual ~IRenderTargetView() = default;

	public:
		virtual void ClearRenderTargetView(UINT _RTVNumber, const float* _ClearColor) const = 0;
		virtual void ClearDepthStencilView(UINT _Flag, float _Depth, UINT _Stencil) const = 0;

		virtual void OMSetRenderTargets() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXRenderTargetView : public IRenderTargetView
		{
		public:
			CDXRenderTargetView(ComPtr<ID3D11DeviceContext>& _Context, const std::vector<ComPtr<ID3D11RenderTargetView>>& _RTV
				, ComPtr<ID3D11DepthStencilView>& _DSV)
				: Context(_Context)
				, RenderTargetViews{}
				, DepthStencilView(_DSV.Get())
			{
				RenderTargetViews.reserve(_RTV.size());
				for (auto& RTV : _RTV)
				{
					RTV->AddRef();
					RenderTargetViews.push_back(RTV.Get());
				}
				DepthStencilView->AddRef();
			}
			~CDXRenderTargetView() 
			{
				for (auto RTV : RenderTargetViews)
					RTV->Release();
				DepthStencilView->Release();
			}

		public:
			void ClearRenderTargetView(UINT _RTVNumber, const float* _ClearColor) const override
			{
				Context->ClearRenderTargetView(RenderTargetViews[_RTVNumber], _ClearColor);
			}
			void OMSetRenderTargets() const override
			{
				Context->OMSetRenderTargets(RenderTargetViews.size(), RenderTargetViews.data(), DepthStencilView);
			}
			void ClearDepthStencilView(UINT _Flag, float _Depth, UINT _Stencil) const override
			{
				Context->ClearDepthStencilView(DepthStencilView, _Flag, _Depth, _Stencil);
			}

		private:
			std::vector<ID3D11RenderTargetView*> RenderTargetViews;
			ID3D11DepthStencilView* DepthStencilView;

			ComPtr<ID3D11DeviceContext>& Context;
		};
	}
	
}

