#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IDepthStencilState
	{
	public:
		IDepthStencilState() {}
		virtual ~IDepthStencilState() {}

	public:
		virtual void OMSetDepthStencilState(UINT InStencilRef) const = 0;

	protected:

	};

	using Microsoft::WRL::ComPtr;
	class DXDepthStencilState : public IDepthStencilState
	{
	public:
		DXDepthStencilState(ComPtr<ID3D11DeviceContext>& InContext, ComPtr<ID3D11DepthStencilState>& InDSS)
			: Context(InContext)
			, DepthStencilState(InDSS.Get())
		{
			DepthStencilState->AddRef();
		}
		~DXDepthStencilState()
		{
			DepthStencilState->Release();
		}

	public:
		void OMSetDepthStencilState(UINT _StencilRef) const override
		{
			Context->OMSetDepthStencilState(DepthStencilState, _StencilRef);
		}

	private:
		ID3D11DepthStencilState* DepthStencilState;
		ComPtr<ID3D11DeviceContext>& Context;

	};
}

