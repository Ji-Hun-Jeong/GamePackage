#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IUnorderedAccessView
	{
		friend class RenderContext;
	public:
		IUnorderedAccessView() = default;
		virtual ~IUnorderedAccessView() = default;

	private:
		virtual void CSSetUnorderedAccessViews(UINT InStartSlot) const = 0;
		/*virtual void UploadBuffer(UINT _Index, const BufferDesc& _BufferDesc) const = 0;
		virtual void DownloadBuffer(UINT _Index, const BufferDesc& _BufferDesc) const = 0;*/

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXUnorderedAccessView : public IUnorderedAccessView
		{
		public:
			CDXUnorderedAccessView(ComPtr<ID3D11DeviceContext> InContext, std::vector<ComPtr<ID3D11Buffer>>&& InBuffers
				, std::vector<ComPtr<ID3D11Buffer>>&& InStagingBuffers
				, std::vector<ComPtr<ID3D11UnorderedAccessView>>&& InUnorderedAccessViews)
				: Context(InContext)
				, Buffers(InBuffers)
				, StagingBuffers(InStagingBuffers)
				, UnorderedAccessViews(InUnorderedAccessViews)
			{}
			~CDXUnorderedAccessView()
			{}

		public:
			void CSSetUnorderedAccessViews(UINT InStartSlot) const override
			{
				Context->CSSetUnorderedAccessViews(InStartSlot, UnorderedAccessViews.size(), UnorderedAccessViews.data()->GetAddressOf(), nullptr);
			}

			/*void UploadBuffer(UINT _Index, const BufferDesc& _BufferDesc) const override
			{
				ID3D11Buffer* StagingBuffer = StagingBuffers[_Index];
				D3D11_MAPPED_SUBRESOURCE MS;
				Context->Map(StagingBuffer, 0, D3D11_MAP_WRITE, 0, &MS);
				memcpy(MS.pData, _BufferDesc.Data, _BufferDesc.ElementSize * _BufferDesc.NumOfElement);
				Context->Unmap(StagingBuffer, 0);

				ID3D11Buffer* Buffer = Buffers[_Index];
				Context->CopyResource(Buffer, StagingBuffer);

			}

			void DownloadBuffer(UINT _Index, const BufferDesc& _BufferDesc) const override
			{
				ID3D11Buffer* Buffer = Buffers[_Index];
				ID3D11Buffer* StagingBuffer = StagingBuffers[_Index];
				Context->CopyResource(StagingBuffer, Buffer);

				D3D11_MAPPED_SUBRESOURCE MS;
				Context->Map(StagingBuffer, 0, D3D11_MAP_READ, 0, &MS);
				memcpy(_BufferDesc.Data, MS.pData, _BufferDesc.ElementSize * _BufferDesc.NumOfElement);
				Context->Unmap(StagingBuffer, 0);
			}*/

		private:
			std::vector<ComPtr<ID3D11Buffer>> Buffers;
			std::vector<ComPtr<ID3D11Buffer>> StagingBuffers;	// Gpu -> Cpu로 읽어오기 위해
			std::vector<ComPtr<ID3D11UnorderedAccessView>> UnorderedAccessViews;
			ComPtr<ID3D11DeviceContext> Context;
		};
	}

}

