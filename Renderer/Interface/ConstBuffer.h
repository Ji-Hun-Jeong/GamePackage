#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	struct CpuConstData
	{
		void* Data;
		UINT Size;
	};
	class IConstBuffer
	{
		friend class RenderContext;
	public:
		IConstBuffer() = default;
		virtual ~IConstBuffer() = default;

	public:
		virtual void UpdateBuffer() const = 0;

	private:
		virtual void VSSetConstBuffers(UINT InStartSlot) const = 0;
		virtual void PSSetConstBuffers(UINT InStartSlot) const = 0;
		virtual void CSSetConstBuffers(UINT InStartSlot) const = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXConstBuffer : public IConstBuffer
		{
		public:
			CDXConstBuffer(ComPtr<ID3D11DeviceContext> InContext, std::vector<ComPtr<ID3D11Buffer>>&& InBuffers
				, std::vector<CpuConstData>&& InCpuData)
				: Context(InContext)
				, Buffers(InBuffers)
				, CpuData(InCpuData)
			{
			}
			~CDXConstBuffer()
			{

			}
		public:
			void UpdateBuffer() const override
			{
				D3D11_MAPPED_SUBRESOURCE Ms;
				ZeroMemory(&Ms, sizeof(Ms));
				for (size_t i = 0; i < Buffers.size(); ++i)
				{
					Context->Map(Buffers[i].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Ms);
					memcpy(Ms.pData, CpuData[i].Data, CpuData[i].Size);
					Context->Unmap(Buffers[i].Get(), 0);
				}
			}

		private:
			void VSSetConstBuffers(UINT InStartSlot) const override
			{
				Context->VSSetConstantBuffers(InStartSlot, UINT(Buffers.size()), Buffers.data()->GetAddressOf());
			}

			void PSSetConstBuffers(UINT InStartSlot) const override
			{
				Context->PSSetConstantBuffers(InStartSlot, UINT(Buffers.size()), Buffers.data()->GetAddressOf());
			}

			void CSSetConstBuffers(UINT InStartSlot) const override
			{
				Context->CSSetConstantBuffers(InStartSlot, UINT(Buffers.size()), Buffers.data()->GetAddressOf());
			}

		private:
			std::vector<ComPtr<ID3D11Buffer>> Buffers;
			std::vector<CpuConstData> CpuData;

			ComPtr<ID3D11DeviceContext> Context;
		};
	}
}

