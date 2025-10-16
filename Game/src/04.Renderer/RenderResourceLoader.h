#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include "PSOManager.h"

class CRenderResourceLoader
{
public:
	CRenderResourceLoader(Graphics::CRenderDevice& InDevice, CPSOManager& InPSOManager)
		: Device(InDevice)
		, PSOManager(InPSOManager)
	{}
	~CRenderResourceLoader() = default;

public:
	Graphics::CMesh* MakeMesh(const Graphics::TMeshData& InMeshData)
	{
		auto Iter = Meshes.find(InMeshData.Key);
		if (Iter != Meshes.end())
			return Iter->second.get();

		assert(InMeshData.VertexBufferInitData.CopyStartPoint);
		assert(InMeshData.IndexBufferInitData.CopyStartPoint);

		auto VertexBuffer = Device.CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		auto IndexBuffer = Device.CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(std::move(VertexBuffer), std::move(IndexBuffer)
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);

		Meshes.emplace(InMeshData.Key, Mesh);
		return Mesh;
	}

	Graphics::CMaterial* MakeMaterial(const Graphics::TMaterialData& InMaterialData)
	{
		auto Iter = Materials.find(InMaterialData.Key);
		if (Iter != Materials.end())
			return Iter->second.get();

		Graphics::CMaterial* Material = new Graphics::CMaterial;
		Materials.emplace(InMaterialData.Key, Material);
		return Material;
	}

	CPSO* GetPSO(EPSOType InPSOType) const { return PSOManager.GetPSO(InPSOType); }

	std::unique_ptr<Graphics::CBuffer> MakeConstBuffer(const Graphics::TBufferDesc& InConstBufferDesc, const Graphics::TBufferInitalizeData* InBufferInitalizeData) const
	{
		return Device.CreateBuffer(InConstBufferDesc, InBufferInitalizeData);
	}

private:
	Graphics::CRenderDevice& Device;
	CPSOManager& PSOManager;

	std::map<Graphics::MeshKey, std::unique_ptr<Graphics::CMesh>> Meshes;
	std::map<Graphics::MaterialKey, std::unique_ptr<Graphics::CMaterial>> Materials;

};

