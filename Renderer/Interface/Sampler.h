#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class ISampler
	{
		friend class RenderContext;
	public:
		ISampler() = default;
		virtual ~ISampler() = default;

	private:
		virtual void VSSetSampler(UINT _StartSlot) const = 0;
		virtual void CSSetSampler(UINT _StartSlot) const = 0;
		virtual void PSSetSampler(UINT _StartSlot) const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXSampler : public ISampler
		{
		public:
			CDXSampler(ComPtr<ID3D11DeviceContext> InContext, std::vector<ComPtr<ID3D11SamplerState>>&& InSamplers)
				: Context(InContext)
				, Samplers(InSamplers)
			{}
			~CDXSampler()
			{}

		public:
			void VSSetSampler(UINT _StartSlot) const override
			{
				Context->VSSetSamplers(_StartSlot, Samplers.size(), Samplers.data()->GetAddressOf());
			}
			void CSSetSampler(UINT _StartSlot) const override
			{
				Context->CSSetSamplers(_StartSlot, Samplers.size(), Samplers.data()->GetAddressOf());
			}
			void PSSetSampler(UINT _StartSlot) const override
			{
				Context->PSSetSamplers(_StartSlot, Samplers.size(), Samplers.data()->GetAddressOf());
			}

		private:
			std::vector<ComPtr<ID3D11SamplerState>> Samplers;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

