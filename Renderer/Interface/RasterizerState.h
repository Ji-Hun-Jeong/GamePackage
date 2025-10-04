#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IRasterizerState
	{
		friend class RenderContext;
	public:
		IRasterizerState() = default;
		virtual ~IRasterizerState() = default;

	private:
		virtual void RSSetState() const = 0;


	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXRasterizerState : public IRasterizerState
		{
		public:
			CDXRasterizerState(ComPtr<ID3D11DeviceContext> InContext, ComPtr<ID3D11RasterizerState> InRS)
				: Context(InContext)
				, RasterizerState(InRS)
			{
			}
			~CDXRasterizerState()
			{
			}

		public:
			void RSSetState() const override
			{
				Context->RSSetState(RasterizerState.Get());
			}

		private:
			ComPtr<ID3D11RasterizerState> RasterizerState;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

