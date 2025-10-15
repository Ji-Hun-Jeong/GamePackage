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
		auto VertexBuffer = Device.CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		auto IndexBuffer = Device.CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(std::move(VertexBuffer), std::move(IndexBuffer)
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);

		Meshes.emplace_back(Mesh);
		return Mesh;
	}
	Graphics::CMaterial* MakeMaterial(const Graphics::TMaterialData& InMaterialData)
	{
		Graphics::CMaterial* Material = new Graphics::CMaterial();
		Materials.emplace_back(Material);
		return Material;
	}
	CPSO* GetPSO(EPSOType InPSOType) const { return PSOManager.GetPSO(InPSOType); }

	Graphics::CBuffer* MakeConstBuffer(const Graphics::TBufferDesc& InConstBufferDesc, const Graphics::TBufferInitalizeData* InBufferInitalizeData)
	{
		auto ConstBuffer = Device.CreateBuffer(InConstBufferDesc, InBufferInitalizeData);
		Graphics::CBuffer* RawBuffer = ConstBuffer.get();
		ConstBuffers.push_back(std::move(ConstBuffer));
		return RawBuffer;
	}

private:
	Graphics::CRenderDevice& Device;
	CPSOManager& PSOManager;

	std::vector<std::unique_ptr<Graphics::CMesh>> Meshes;
	std::vector<std::unique_ptr<Graphics::CMaterial>> Materials;
	std::vector<std::unique_ptr<Graphics::CBuffer>> ConstBuffers;
};

