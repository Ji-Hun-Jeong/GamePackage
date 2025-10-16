#pragma once
#include "Component.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
	GenerateObject()
public:
	CRenderComponent()
		: RenderStateObject(nullptr)
		, RenderResourceLoader(nullptr)
	{}
	~CRenderComponent() = default;

public:
	void SetMesh(const Graphics::TMeshData& InMeshData)
	{
		Graphics::CMesh* Mesh = RenderResourceLoader->MakeMesh(InMeshData);
		RenderStateObject->SetMesh(Mesh);
	}
	void SetMaterial(const Graphics::TMaterialData& InMaterialData)
	{
		Graphics::CMaterial* Material = RenderResourceLoader->MakeMaterial(InMaterialData);
		RenderStateObject->SetMaterial(Material);
	}
	void SetPSO(EPSOType InPSOType)
	{
		CPSO* PSO = RenderResourceLoader->GetPSO(InPSOType);
		RenderStateObject->SetPSO(PSO);
	}
	void AddVertexConstBuffer(const Graphics::TBufferDesc& InBufferDesc)
	{
		std::unique_ptr<Graphics::CBuffer> VertexConstBuffer = RenderResourceLoader->MakeConstBuffer(InBufferDesc, nullptr);
		auto VertexConstBufferMapInstance = RenderStateObject->AddVertexConstBuffer(std::move(VertexConstBuffer));
		VertexConstBufferMapInstances.push_back(std::move(VertexConstBufferMapInstance));
	}
	void UpdateVertexConstBuffer(size_t InIndex, const void* InMapDataPoint, size_t InDataSize)
	{
		// assert(InBufferMapInstance);
		RenderStateObject->UpdateVertexConstBuffer(VertexConstBufferMapInstances[InIndex].get(), InMapDataPoint, InDataSize);
	}

public:
	void SetRenderStateObject(CRenderStateObject* InRenderStateObject)
	{
		assert(InRenderStateObject);
		RenderStateObject = InRenderStateObject;
	}
	void SetRenderResourceLoader(CRenderResourceLoader* InRenderResourceLoader)
	{
		assert(InRenderResourceLoader);
		RenderResourceLoader = InRenderResourceLoader;
	}

private:
	CRenderStateObject* RenderStateObject;
	CRenderResourceLoader* RenderResourceLoader;

	std::vector<std::unique_ptr<CBufferMapInstance>> VertexConstBufferMapInstances;
};

