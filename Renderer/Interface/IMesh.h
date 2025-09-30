#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	class IMesh
	{
	public:
		IMesh() = default;
		virtual ~IMesh() = default;

	public:
		virtual void IASetBuffer(UINT _StartSlot) const = 0;
		virtual void DrawIndexed() const = 0;

	protected:

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXModel : public IMesh
		{
		public:
			CDXModel(ComPtr<ID3D11DeviceContext>& _Context, const std::vector<ComPtr<ID3D11Buffer>>& _VB
				, ComPtr<ID3D11Buffer>& _IB, DXGI_FORMAT _Format, UINT _Stride, UINT _Offset, UINT _IndexCount)
				: Context(_Context)
				, VertexBuffers{}
				, IndexBuffer(_IB.Get())
				, Format(_Format)
				, Stride(_Stride)
				, Offset(_Offset)
				, IndexCount(_IndexCount)
			{
				for (auto& VB : _VB)
				{
					VB->AddRef();
					VertexBuffers.push_back(VB.Get());
				}
				IndexBuffer->AddRef();
			}
			~CDXModel()
			{
				for (auto VB : VertexBuffers)
					VB->Release();
				if (IndexBuffer)
					IndexBuffer->Release();

			}

		public:
			void IASetBuffer(UINT _StartSlot) const override
			{
				Context->IASetVertexBuffers(_StartSlot, VertexBuffers.size(), VertexBuffers.data(), &Stride, &Offset);
				Context->IASetIndexBuffer(IndexBuffer, Format, 0);
			}
			void DrawIndexed() const override
			{
				Context->DrawIndexed(IndexCount, 0, 0);
			}

		private:
			std::vector<ID3D11Buffer*> VertexBuffers;
			ID3D11Buffer* IndexBuffer;

			DXGI_FORMAT Format;

			UINT Stride;
			UINT Offset;
			UINT IndexCount;

			ComPtr<ID3D11DeviceContext>& Context;
		};
	}

}

