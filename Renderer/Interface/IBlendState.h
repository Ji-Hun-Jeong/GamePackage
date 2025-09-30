#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IBlendState
	{
	public:
		IBlendState() = default;
		virtual ~IBlendState() = default;

	public:
		virtual void OMSetBlendState() const = 0;

	};

	using Microsoft::WRL::ComPtr;
	class CDXBlendState : public IBlendState
	{
	public:
		CDXBlendState(ComPtr<ID3D11DeviceContext>& InContext, ComPtr<ID3D11BlendState>&& InBlendState, const float* InBlendFactor)
			: Context(InContext)
			, BlendState(InBlendState)
			, BlendFactor{ 0 }
		{
			memcpy(BlendFactor, InBlendFactor, 4);
		}
		~CDXBlendState() = default;

	public:
		void OMSetBlendState() const override
		{
			Context->OMSetBlendState(BlendState.Get(), BlendFactor, 0xffffffff);
		}

	private:
		ComPtr<ID3D11DeviceContext>& Context;
		ComPtr<ID3D11BlendState> BlendState;

		float BlendFactor[4];
	};

}

