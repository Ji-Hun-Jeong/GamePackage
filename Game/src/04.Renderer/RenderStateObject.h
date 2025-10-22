#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/RHI/Buffer.h>
#include "Image.h"
#include "PSOManager.h"

class CBufferMapInstance
{
	friend class CRenderStateObject;
	CBufferMapInstance(size_t InMappingIndex)
		: MappingIndex(InMappingIndex)
		, MapDataPoint(nullptr)
		, DataSize(0)
	{}
public:
	~CBufferMapInstance() = default;

public:
	size_t GetMappingIndex() const { return MappingIndex; }

private:
	size_t MappingIndex;

	const void* MapDataPoint;
	size_t DataSize;

};

class CRenderStateObject
{
	friend class CSpriteRenderer;
	CRenderStateObject()
		: Mesh(nullptr)
		, PSO(nullptr)
		, bDestroy(false)
		, VertexShaderResources{}
		, PixelShaderResources{}
		, PixelShaderResourceStartSlot(0)
		, VertexConstBufferStartSlot(0)
		, bRender(true)
	{}
public:
	~CRenderStateObject() = default;

public:
	void SetMesh(Graphics::CMesh* InMesh)
	{
		if (InMesh == nullptr)std::cout << "Mesh Is None\n";
		Mesh = InMesh;
	}
	void SetPixelShaderResource(uint8_t InPixelShaderResourceSlot, CImage* InImage)
	{
		assert(InPixelShaderResourceSlot < 12);
		PixelShaderResources[InPixelShaderResourceSlot] = InImage;
	}
	void SetVertexShaderResource(uint8_t InVertexShaderResourceSlot, CImage* InImage)
	{
		// 구현해야함
		assert(0);
		assert(InImage);
		VertexShaderResources[InVertexShaderResourceSlot] = InImage;
	}
	void SetPSO(CPSO* InPSO)
	{
		assert(InPSO);
		PSO = InPSO;
	}
	void SetPixelShaderResourceStartSlot(uint32_t InPixelShaderResourceStartSlot) { PixelShaderResourceStartSlot = InPixelShaderResourceStartSlot; }
	void SetVertexConstBufferStartSlot(uint32_t InVertexConstBufferStartSlot)
	{
		VertexConstBufferStartSlot = InVertexConstBufferStartSlot;
	}

	std::unique_ptr<CBufferMapInstance> AddVertexConstBuffer(std::unique_ptr<Graphics::CBuffer> InVertexConstBuffer, const void* InMapDataPoint = nullptr, size_t InDataSize = 0)
	{
		assert(InVertexConstBuffer);
		VertexConstBuffers.push_back(std::move(InVertexConstBuffer));
		size_t Index = VertexConstBuffers.size() - 1;

		CBufferMapInstance* BufferMapInstance = new CBufferMapInstance(Index);
		if (InMapDataPoint)
			UpdateVertexConstBuffer(BufferMapInstance, InMapDataPoint, InDataSize);

		return std::unique_ptr<CBufferMapInstance>(BufferMapInstance);
	}

	void UpdateVertexConstBuffer(CBufferMapInstance* InBufferMapInstance, const void* InMapDataPoint, size_t InDataSize)
	{
		InBufferMapInstance->MapDataPoint = InMapDataPoint;
		InBufferMapInstance->DataSize = InDataSize;
		BufferMapInstances.push(InBufferMapInstance);
	}

	void BindRenderState(Graphics::CRenderContext& InContext);
	void Destroy() { bDestroy = true; }
	void SetRender(bool bInRender) { bRender = bInRender; }

protected:
	Graphics::CMesh* Mesh;
	CPSO* PSO;

	std::array<CImage*, 6> VertexShaderResources;
	std::array<CImage*, 6> PixelShaderResources;

	std::vector<std::unique_ptr<Graphics::CBuffer>> VertexConstBuffers;
	std::queue<const CBufferMapInstance*> BufferMapInstances;

	bool bDestroy;

	uint32_t PixelShaderResourceStartSlot;
	uint32_t VertexConstBufferStartSlot;

	bool bRender;
};