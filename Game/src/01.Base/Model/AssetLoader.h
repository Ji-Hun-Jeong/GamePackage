#pragma once
#include "Model.h"
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/MeshManager.h>

class CAssetLoader
{
	SINGLE(CAssetLoader)
public:
	void Initalize(Graphics::CRenderDevice* InDevice, Graphics::CMeshManager* InMeshManager)
	{
		Device = InDevice;
	}
	void Finalize()
	{
		Models.clear();
	}

public:
	CModel* MakeModel(const std::string& InModelName, const Graphics::TMeshData& InMeshData, const Graphics::TMaterialData& InMaterialData)
	{
		auto Iter = Models.find(InModelName);
		if (Iter != Models.end())
			return Iter->second.get();

		auto VertexBuffer = Device->CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		auto IndexBuffer = Device->CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		auto Mesh = std::make_unique<Graphics::CMesh>(std::move(VertexBuffer), std::move(IndexBuffer), InMeshData.PrimitiveTopology
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);
		auto Material = std::make_unique<Graphics::CMaterial>();

		CModel* Model = new CModel(std::move(Mesh), std::move(Material));
		Models.emplace(InModelName, Model);
		return Model;
	}

private:
	Graphics::CRenderDevice* Device;

	std::map<std::string, std::unique_ptr<CModel>> Models;

};

