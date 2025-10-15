#pragma once
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include <Renderer/RHI/Buffer.h>
#include "PSOManager.h"

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

	void AddVertexConstBuffer(Graphics::CBuffer* InVertexConstBuffer, const Graphics::TBufferMapResource& InVertexBufferMapResource)
	{
		assert(InVertexConstBuffer);
		VertexConstBuffers.push_back(InVertexConstBuffer);
		VertexBufferMapResources.push_back(InVertexBufferMapResource);
		size_t Index = VertexConstBuffers.size() - 1;
		UpdateVertexConstBuffer(uint32_t(Index));
	}
	void UpdateVertexConstBuffer(uint32_t Index)
	{
		UpdateBufferIndexs.push(uint32_t(Index));
	}

	void BindRenderState(Graphics::CRenderContext& InContext);
	void Destroy() { bDestroy = true; }

protected:
	Graphics::CMesh* Mesh;
	Graphics::CMaterial* Material;
	CPSO* PSO;

	std::vector<Graphics::CBuffer*> VertexConstBuffers;
	std::vector<Graphics::TBufferMapResource> VertexBufferMapResources;
	std::queue<uint32_t> UpdateBufferIndexs;

	bool bDestroy;

};