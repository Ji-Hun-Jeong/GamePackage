#pragma once
#include <queue>
#include "RHI/Buffer.h"

namespace Graphics
{
	using MeshKey = uint32_t;
	struct TMeshData
	{
		std::vector<uint8_t> Vertices;
		std::vector<uint8_t> Indices;

		EGIFormat IndexFormat = EGIFormat::GI_FORMAT_R32_UINT;
		uint32_t IndexCount = 0;
		uint32_t Stride = 0;
		uint32_t Offset = 0;

		MeshKey Key = 0;
	};



	class CMesh
	{
	public:
		CMesh(std::unique_ptr<CBuffer> InVertexBuffer, std::unique_ptr<CBuffer> InIndexBuffer
			, EGIFormat InIndexFormat, uint32_t InIndexCount, uint32_t InStride, uint32_t InOffset)
			: VertexBuffer(std::move(InVertexBuffer))
			, IndexBuffer(std::move(InIndexBuffer))
			, IndexFormat(InIndexFormat)
			, IndexCount(InIndexCount)
			, Stride(InStride)
			, Offset(InOffset)
		{}
		~CMesh() 
		{
		}

	public:
		const CBuffer& GetVertexBuffer() const { return *VertexBuffer.get(); }
		const CBuffer& GetIndexBuffer() const { return *IndexBuffer.get(); }
		EGIFormat GetIndexFormat() const { return IndexFormat; }
		uint32_t GetIndexCount() const { return IndexCount; }
		const uint32_t& GetStride() const { return Stride; }
		const uint32_t& GetOffset() const { return Offset; }

	private:
		std::unique_ptr<CBuffer> VertexBuffer;
		std::unique_ptr<CBuffer> IndexBuffer;

		EGIFormat IndexFormat;
		uint32_t IndexCount;
		uint32_t Stride;
		uint32_t Offset;

	};
}


