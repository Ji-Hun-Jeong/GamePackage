#pragma once
#include "RHI/Buffer.h"

namespace Graphics
{
	struct TMeshData
	{
		Graphics::TBufferDesc VertexBufferDesc;
		Graphics::TBufferInitalizeData VertexBufferInitData;
		Graphics::TBufferDesc IndexBufferDesc;
		Graphics::TBufferInitalizeData IndexBufferInitData;

		Graphics::EGIFormat IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		uint32_t IndexCount = 0;
		uint32_t Stride = 0;
		uint32_t Offset = 0;
	};

	class CMesh
	{
	public:
		CMesh(CBuffer& InVertexBuffer, CBuffer& InIndexBuffer, EGIFormat InIndexFormat, uint32_t InIndexCount
			, uint32_t InStride, uint32_t InOffset)
			: VertexBuffer(InVertexBuffer)
			, IndexBuffer(InIndexBuffer)
			, IndexFormat(InIndexFormat)
			, IndexCount(InIndexCount)
			, Stride(InStride)
			, Offset(InOffset)
		{}
		~CMesh() {}

	public:
		void BindToPipeline(class CRenderContext& InContext);
		uint32_t GetIndexCount() const { return IndexCount; }

	private:
		CBuffer& VertexBuffer;
		CBuffer& IndexBuffer;
		EGIFormat IndexFormat;
		uint32_t IndexCount;
		uint32_t Stride;
		uint32_t Offset;

	};
}


