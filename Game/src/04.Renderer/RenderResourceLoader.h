#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/Mesh.h>
#include "PSOManager.h"
#include "03.Utils/GeometryGenerator.h"
#include "Image.h"

class CRenderResourceLoader
{
public:
	CRenderResourceLoader(Graphics::CRenderDevice& InDevice, CPSOManager& InPSOManager)
		: Device(InDevice)
		, PSOManager(InPSOManager)
	{}
	~CRenderResourceLoader() = default;

public:
	void LoadMesh(const Graphics::TMeshData& InMeshData)
	{
		auto Iter = Meshes.find(InMeshData.Key);
		if (Iter != Meshes.end())
			return;

		assert(InMeshData.VertexBufferInitData.CopyStartPoint);
		assert(InMeshData.IndexBufferInitData.CopyStartPoint);

		auto VertexBuffer = Device.CreateBuffer(InMeshData.VertexBufferDesc, &InMeshData.VertexBufferInitData);
		auto IndexBuffer = Device.CreateBuffer(InMeshData.IndexBufferDesc, &InMeshData.IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(std::move(VertexBuffer), std::move(IndexBuffer)
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);

		Meshes.emplace(InMeshData.Key, Mesh);
	}

	Graphics::CMesh* GetMeshOrNull(Graphics::MeshKey InMeshKey)
	{
		auto Iter = Meshes.find(InMeshKey);

		if (Iter == Meshes.end())
			return nullptr;

		return Iter->second.get();
	}

	CImage* LoadImageFromFile(const std::wstring& InPath)
	{
		auto Iter = Images.find(InPath);
		if (Iter != Images.end())
			return Iter->second.get();

		auto SRV_Texture2D = Device.CreateImage(InPath);

		CImage* RawImage = new CImage(std::move(SRV_Texture2D.first), std::move(SRV_Texture2D.second));
		Images.emplace(InPath, RawImage);

		return RawImage;
	}

	CPSO* GetPSO(EPSOType InPSOType) const { return PSOManager.GetPSO(InPSOType); }

	std::unique_ptr<Graphics::CBuffer> CreateConstBuffer(const Graphics::TBufferDesc& InConstBufferDesc, const Graphics::TBufferInitalizeData* InBufferInitalizeData) const
	{
		return Device.CreateBuffer(InConstBufferDesc, InBufferInitalizeData);
	}

private:
	Graphics::CRenderDevice& Device;
	CPSOManager& PSOManager;

	std::map<Graphics::MeshKey, std::unique_ptr<Graphics::CMesh>> Meshes;
	std::map<std::wstring, std::unique_ptr<CImage>> Images;

};

