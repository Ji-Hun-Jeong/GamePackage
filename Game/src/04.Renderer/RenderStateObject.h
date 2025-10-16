#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include <Renderer/RHI/Buffer.h>
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
	friend class CRenderer;
	CRenderStateObject()
		: Mesh(nullptr)
		, Material(nullptr)
		, PSO(nullptr)
		, bDestroy(false)
	{}
public:
	~CRenderStateObject() = default;

public:
	void SetMesh(Graphics::CMesh* InMesh) { Mesh = InMesh; }
	void SetMaterial(Graphics::CMaterial* InMaterial) { Material = InMaterial; }
	void SetPSO(CPSO* InPSO) { PSO = InPSO; }

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

protected:
	Graphics::CMesh* Mesh;
	Graphics::CMaterial* Material;
	CPSO* PSO;

	std::vector<std::unique_ptr<Graphics::CBuffer>> VertexConstBuffers;
	std::queue<const CBufferMapInstance*> BufferMapInstances;

	bool bDestroy;

};