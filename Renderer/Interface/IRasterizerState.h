#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IRasterizerState
	{
	public:
		IRasterizerState() = default;
		virtual ~IRasterizerState() = default;

	public:
		virtual void RSSetState() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXRasterizerState : public IRasterizerState
		{
		public:
			CDXRasterizerState(ComPtr<ID3D11DeviceContext>& _Context, ComPtr<ID3D11RasterizerState>& _RS)
				: Context(_Context)
				, RasterizerState(_RS.Get())
			{
				RasterizerState->AddRef();
			}
			~CDXRasterizerState()
			{
				RasterizerState->Release();
			}

		public:
			void RSSetState() const override
			{
				Context->RSSetState(RasterizerState);
			}

		private:
			ID3D11RasterizerState* RasterizerState;
			ComPtr<ID3D11DeviceContext>& Context;
		};
	}

}

