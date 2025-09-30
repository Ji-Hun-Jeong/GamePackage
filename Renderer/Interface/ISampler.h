#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class ISampler
	{
	public:
		ISampler() = default;
		virtual ~ISampler() = default;

	public:
		virtual void VSSetSampler(UINT _StartSlot) const = 0;
		virtual void CSSetSampler(UINT _StartSlot) const = 0;
		virtual void PSSetSampler(UINT _StartSlot) const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXSampler : public ISampler
		{
		public:
			CDXSampler(ComPtr<ID3D11DeviceContext>& _Context, const std::vector<ComPtr<ID3D11SamplerState>>& _SS)
				: Context(_Context)
				, Samplers{}
			{
				for (auto& SS : _SS)
				{
					SS->AddRef();
					Samplers.push_back(SS.Get());
				}
			}
			~CDXSampler()
			{
				for (auto SS : Samplers)
					SS->Release();
			}

		public:
			void VSSetSampler(UINT _StartSlot) const override
			{
				Context->VSSetSamplers(_StartSlot, Samplers.size(), Samplers.data());
			}
			void CSSetSampler(UINT _StartSlot) const override
			{
				Context->CSSetSamplers(_StartSlot, Samplers.size(), Samplers.data());
			}
			void PSSetSampler(UINT _StartSlot) const override
			{
				Context->PSSetSamplers(_StartSlot, Samplers.size(), Samplers.data());
			}

		private:
			std::vector<ID3D11SamplerState*> Samplers;
			ComPtr<ID3D11DeviceContext>& Context;
		};
	}

}

