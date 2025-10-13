#pragma once
#include "RHI/Buffer.h"

namespace Graphics
{
	struct TMeshData
	{
		TBufferDesc VertexBufferDesc;
		TBufferInitalizeData VertexBufferInitData;
		TBufferDesc IndexBufferDesc;
		TBufferInitalizeData IndexBufferInitData;

		ETopology PrimitiveTopology = ETopology::PrimitiveTopologyTRIANGLELIST;
		EGIFormat IndexFormat = EGIFormat::GI_FORMAT_R32_UINT;
		uint32_t IndexCount = 0;
		uint32_t Stride = 0;
		uint32_t Offset = 0;
	};

	class CMesh
	{
	public:
		CMesh(std::unique_ptr<CBuffer> InVertexBuffer, std::unique_ptr<CBuffer> InIndexBuffer, ETopology InPrimitiveTopology
			, EGIFormat InIndexFormat, uint32_t InIndexCount, uint32_t InStride, uint32_t InOffset)
			: VertexBuffer(std::move(InVertexBuffer))
			, IndexBuffer(std::move(InIndexBuffer))
			, PrimitiveTopology(InPrimitiveTopology)
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
		std::unique_ptr<CBuffer> VertexBuffer;
		std::unique_ptr<CBuffer> IndexBuffer;
		ETopology PrimitiveTopology;
		EGIFormat IndexFormat;
		uint32_t IndexCount;
		uint32_t Stride;
		uint32_t Offset;

	};
}


