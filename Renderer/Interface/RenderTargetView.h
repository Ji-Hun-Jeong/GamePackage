#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IRenderTargetView
	{
		friend class RenderContext;
	public:
		IRenderTargetView() = default;
		virtual ~IRenderTargetView() = default;

	private:
		virtual void ClearRenderTargetView(UINT InRTVNumber, const float* InClearColor) const = 0;
		virtual void ClearDepthStencilView(UINT InFlag, float InDepth, UINT InStencil) const = 0;

		virtual void OMSetRenderTargets() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXRenderTargetView : public IRenderTargetView
		{
		public:
			CDXRenderTargetView(ComPtr<ID3D11DeviceContext> InContext, std::vector<ComPtr<ID3D11RenderTargetView>>&& InRTV
				, ComPtr<ID3D11DepthStencilView> InDSV)
				: Context(InContext)
				, RenderTargetViews(InRTV)
				, DepthStencilView(InDSV.Get())
			{
			}
			~CDXRenderTargetView() 
			{
			}

		public:
			void ClearRenderTargetView(UINT InRTVNumber, const float* InClearColor) const override
			{
				Context->ClearRenderTargetView(RenderTargetViews[InRTVNumber].Get(), InClearColor);
			}
			void OMSetRenderTargets() const override
			{
				Context->OMSetRenderTargets(RenderTargetViews.size(), RenderTargetViews.data()->GetAddressOf(), DepthStencilView.Get());
			}
			void ClearDepthStencilView(UINT InFlag, float InDepth, UINT InStencil) const override
			{
				Context->ClearDepthStencilView(DepthStencilView.Get(), InFlag, InDepth, InStencil);
			}

		private:
			std::vector<ComPtr<ID3D11RenderTargetView>> RenderTargetViews;
			ComPtr<ID3D11DepthStencilView> DepthStencilView;

			ComPtr<ID3D11DeviceContext> Context;
		};
	}
	
}

