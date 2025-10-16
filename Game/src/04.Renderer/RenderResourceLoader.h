#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include "PSOManager.h"
#include "03.Utils/GeometryGenerator.h"

struct TModel
{
	Graphics::TMeshData MeshData;
	Graphics::TMaterialData MaterialData;
};

class CRenderResourceLoader
{
public:
	CRenderResourceLoader(Graphics::CRenderDevice& InDevice, CPSOManager& InPSOManager, uint32_t InScreenWidth, uint32_t InScreenHeight)
		: Device(InDevice)
		, PSOManager(InPSOManager)
		, ScreenWidth(InScreenWidth)
		, ScreenHeight(InScreenHeight)
	{}
	~CRenderResourceLoader() = default;

public:
	Graphics::CMesh* LoadMesh(const Graphics::TMeshData& InMeshData)
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

	Graphics::CMaterial* LoadMaterial(const Graphics::TMaterialData& InMaterialData)
	{
		auto Iter = Materials.find(InMaterialData.Key);
		if (Iter != Materials.end())
			return Iter->second.get();

		Graphics::CMaterial* Material = new Graphics::CMaterial;
		if (InMaterialData.ImagePath.empty() == false)
		{
			auto SRV = Device.CreateImage(InMaterialData.ImagePath);
			Material->AddShaderResourceView(std::move(SRV));
		}

		Materials.emplace(InMaterialData.Key, Material);
		return Material;
	}

	CPSO* GetPSO(EPSOType InPSOType) const { return PSOManager.GetPSO(InPSOType); }

	std::unique_ptr<Graphics::CBuffer> CreateConstBuffer(const Graphics::TBufferDesc& InConstBufferDesc, const Graphics::TBufferInitalizeData* InBufferInitalizeData) const
	{
		return Device.CreateBuffer(InConstBufferDesc, InBufferInitalizeData);
	}

	TModel LoadModelFromFile(const std::wstring& InPath) const
	{
		auto ShaderResourceView = Device.CreateImage(InPath);
		const Graphics::TTexture2DDesc& ImageDesc = ShaderResourceView->GetTexture2D().GetTexture2DDesc();
		const uint32_t ImageWidth = ImageDesc.Width;
		const uint32_t ImageHeight = ImageDesc.Height;

		TGeometryData GeometryData = CGeometryGenerator::GenerateSquare(float(ScreenWidth), float(ScreenHeight), float(ImageWidth), float(ImageHeight));

		Graphics::TMeshData MeshData;
		MeshData.VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
		MeshData.VertexBufferDesc.ByteWidth = uint32_t(sizeof(TVertex) * GeometryData.Vertices.size());
		MeshData.VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		MeshData.VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;
		MeshData.VertexBufferInitData.CopyStartPoint = GeometryData.Vertices.data();

		MeshData.IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
		MeshData.IndexBufferDesc.ByteWidth = uint32_t(sizeof(uint32_t) * GeometryData.Indices.size());
		MeshData.IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		MeshData.IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;
		MeshData.IndexBufferInitData.CopyStartPoint = GeometryData.Indices.data();

		MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		MeshData.IndexCount = uint32_t(GeometryData.Indices.size());
		MeshData.Stride = sizeof(TVertex);
		MeshData.Offset = 0;
		MeshData.Key = 1;

		Graphics::TMaterialData MaterialData;
		MaterialData.ImagePath = InPath;
		MaterialData.Key = 0;
	}

private:
	Graphics::CRenderDevice& Device;
	CPSOManager& PSOManager;

	std::map<Graphics::MeshKey, std::unique_ptr<Graphics::CMesh>> Meshes;
	std::map<Graphics::MaterialKey, std::unique_ptr<Graphics::CMaterial>> Materials;

	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

};

