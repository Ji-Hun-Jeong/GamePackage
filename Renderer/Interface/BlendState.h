#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IBlendState
	{
		friend class RenderContext;
	public:
		IBlendState() = default;
		virtual ~IBlendState() = default;

	private:
		virtual void OMSetBlendState(UINT InSampleMask = 0xffffffff) const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXBlendState : public IBlendState
		{
		public:
			CDXBlendState(ComPtr<ID3D11DeviceContext> InContext, ComPtr<ID3D11BlendState> InBlendState, const float* InBlendFactor)
				: Context(InContext)
				, BlendState(InBlendState)
				, BlendFactor{ 0 }
			{
				memcpy(BlendFactor, InBlendFactor, sizeof(float));
			}
			~CDXBlendState() = default;

		private:
			void OMSetBlendState(UINT InSampleMask) const override
			{
				Context->OMSetBlendState(BlendState.Get(), BlendFactor, InSampleMask);
			}

		private:
			ComPtr<ID3D11DeviceContext> Context;
			ComPtr<ID3D11BlendState> BlendState;

			float BlendFactor[4];
		};
	}
}

