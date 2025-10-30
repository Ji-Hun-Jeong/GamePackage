#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/Mesh.h>
#include "03.Utils/GeometryGenerator.h"
#include "Image.h"

class CRenderResourceLoader
{
public:
	CRenderResourceLoader(Graphics::CRenderDevice& InDevice)
		: Device(InDevice)
	{}
	~CRenderResourceLoader() = default;

public:
	Graphics::CMesh* LoadMesh(const Graphics::TMeshData& InMeshData)
	{
		auto Iter = Meshes.find(InMeshData.Key);

		if (Iter != Meshes.end())
			return Iter->second.get();

		Graphics::TBufferDesc VertexBufferDesc;
		VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
		VertexBufferDesc.ByteWidth = uint32_t(sizeof(TVertex) * InMeshData.Vertices.size());
		VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		Graphics::TBufferInitalizeData VertexBufferInitData;
		VertexBufferInitData.CopyStartPoint = InMeshData.Vertices.data();
		
		Graphics::TBufferDesc IndexBufferDesc;
		IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
		IndexBufferDesc.ByteWidth = uint32_t(sizeof(uint32_t) * InMeshData.Indices.size());
		IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		Graphics::TBufferInitalizeData IndexBufferInitData;
		IndexBufferInitData.CopyStartPoint = InMeshData.Indices.data();

		auto VertexBuffer = Device.CreateBuffer(VertexBufferDesc, &VertexBufferInitData);
		auto IndexBuffer = Device.CreateBuffer(IndexBufferDesc, &IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(std::move(VertexBuffer), std::move(IndexBuffer)
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);

		Meshes.emplace(InMeshData.Key, Mesh);

		return Mesh;
	}

	CImage* LoadImageFromFile(const std::wstring& InPath)
	{
		auto Iter = Images.find(InPath);
		if (Iter != Images.end())
			return Iter->second.get();

		auto SRV_Texture2D = Device.CreateImage(InPath);

		CImage* RawImage = new CImage(std::move(SRV_Texture2D.first), std::move(SRV_Texture2D.second));
		Images.emplace(InPath, RawImage);

		assert(RawImage);

		return RawImage;
	}

	std::unique_ptr<Graphics::CBuffer> CreateConstBuffer(size_t InByteWidth) const
	{
		Graphics::TBufferDesc ConstBufferDesc;
		ConstBufferDesc.ByteWidth = uint32_t(InByteWidth);
		ConstBufferDesc.Usage = Graphics::EUsage::UsageDynamic;
		ConstBufferDesc.BindFlags = Graphics::EBindFlags::BindConstantBuffer;
		ConstBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;
		return Device.CreateBuffer(ConstBufferDesc, nullptr);
	}

private:
	Graphics::CRenderDevice& Device;

	std::map<Graphics::MeshKey, std::unique_ptr<Graphics::CMesh>> Meshes;
	std::map<std::wstring, std::unique_ptr<CImage>> Images;

};

