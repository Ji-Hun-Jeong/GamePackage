#pragma once
#include "Platform/DX/DX.h"

namespace Graphics
{
	enum class EGIFormat
	{

	};
	class IBuffer
	{
	public:
		IBuffer() = default;
		virtual ~IBuffer() = default;

	private:
		virtual void IASetVertexBuffer(UINT InStartSlot, const UINT* InStride, const UINT* InOffset) = 0;

	};

	namespace DX
	{
		using Microsoft::WRL::ComPtr;
		class CDXBuffer : public IBuffer
		{
		public:
			CDXBuffer() = default;
			~CDXBuffer() = default;

		public:
			void IASetVertexBuffer(UINT InStartSlot, const UINT* InStride, const UINT* InOffset) override
			{
				Context->IASetVertexBuffers(InStartSlot, 1, Buffer.GetAddressOf(), InStride, InOffset);
			}
			void IASetIndexBuffer(EGIFormat InGIFormat, UINT InOffset)
			{
				DXGI_FORMAT Format;
				// Todo: Format Switch
				Context->IASetIndexBuffer(Buffer.Get(), Format, InOffset);
			}
			

		private:
			ComPtr<ID3D11DeviceContext> Context;
			ComPtr<ID3D11Buffer> Buffer;

		};
	}
}
