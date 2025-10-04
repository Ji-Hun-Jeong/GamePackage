#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IDepthStencilState
	{
		friend class RenderContext;
	public:
		IDepthStencilState() {}
		virtual ~IDepthStencilState() {}

	private:
		virtual void OMSetDepthStencilState(UINT InStencilRef) const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXDepthStencilState : public IDepthStencilState
		{
		public:
			CDXDepthStencilState(ComPtr<ID3D11DeviceContext> InContext, ComPtr<ID3D11DepthStencilState> InDSS)
				: Context(InContext)
				, DepthStencilState(InDSS)
			{
			}
			~CDXDepthStencilState()
			{
			}

		public:
			void OMSetDepthStencilState(UINT _StencilRef) const override
			{
				Context->OMSetDepthStencilState(DepthStencilState.Get(), _StencilRef);
			}

		private:
			ComPtr<ID3D11DepthStencilState> DepthStencilState;
			ComPtr<ID3D11DeviceContext> Context;

		};
	}
}

