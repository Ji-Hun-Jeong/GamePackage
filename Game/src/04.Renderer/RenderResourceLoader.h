#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Rendering/Mesh.h>
#include <Renderer/Rendering/Material.h>
#include "03.Utils/GeometryGenerator.h"
#include "04.Renderer/Image.h"

class CRenderResourceLoader
{
	SINGLE(CRenderResourceLoader)
public:
	void Initalize(Graphics::CRenderDevice& InDevice)
	{
		Device = &InDevice;
	}
	void Finalize()
	{
		Meshes.clear();
		Materials.clear();
		PixelShaders.clear();
		Images.clear();
	}
	CImage* GetImage(const std::wstring& InImagePath)
	{
		if (Images.contains(InImagePath) == false)
		{
			auto SRV_Texture2D = Device->CreateImage(InImagePath);
			Images.emplace(InImagePath, std::make_unique<CImage>(std::move(SRV_Texture2D.first), std::move(SRV_Texture2D.second), InImagePath));
		}
		auto Iter = Images.find(InImagePath);
		if (Iter == Images.end())
			assert(0);
		return Iter->second.get();
	}
	Graphics::CMesh* LoadMesh(const Graphics::TMeshData& InMeshData)
	{
		auto Iter = Meshes.find(InMeshData.Key);

		if (Iter != Meshes.end())
			return Iter->second.get();

		Graphics::TBufferDesc VertexBufferDesc;
		VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
		VertexBufferDesc.ByteWidth = uint32_t(InMeshData.Vertices.size());
		VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		Graphics::TBufferInitalizeData VertexBufferInitData;
		VertexBufferInitData.CopyStartPoint = InMeshData.Vertices.data();
		
		Graphics::TBufferDesc IndexBufferDesc;
		IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
		IndexBufferDesc.ByteWidth = uint32_t(InMeshData.Indices.size());
		IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		Graphics::TBufferInitalizeData IndexBufferInitData;
		IndexBufferInitData.CopyStartPoint = InMeshData.Indices.data();

		auto VertexBuffer = Device->CreateBuffer(VertexBufferDesc, &VertexBufferInitData);
		auto IndexBuffer = Device->CreateBuffer(IndexBufferDesc, &IndexBufferInitData);

		Graphics::CMesh* Mesh = new Graphics::CMesh(std::move(VertexBuffer), std::move(IndexBuffer)
			, InMeshData.IndexFormat, InMeshData.IndexCount, InMeshData.Stride, InMeshData.Offset);

		Meshes.emplace(InMeshData.Key, Mesh);

		return Mesh;
	}

	Graphics::CMaterial* LoadMaterial(const Graphics::TMaterialData& InMaterialData)
	{
		auto MaterialIter = Materials.find(InMaterialData);
		if (MaterialIter != Materials.end())
			return MaterialIter->second.get();

		if (PixelShaders.contains(InMaterialData.PixelShaderPath) == false)
		{
			auto PixelShader = Device->CreatePixelShader(InMaterialData.PixelShaderPath);
			PixelShaders.emplace(InMaterialData.PixelShaderPath, std::move(PixelShader));
		}

		for (auto& ImagePath : InMaterialData.ImagePaths)
		{
			if (ImagePath.empty())
				continue;
			if (Images.contains(ImagePath))
				continue;
			auto SRV_Texture2D = Device->CreateImage(ImagePath);
			Images.emplace( ImagePath, std::make_unique<CImage>(std::move(SRV_Texture2D.first), std::move(SRV_Texture2D.second), ImagePath) );
		}

		auto& PixelShader = PixelShaders.find(InMaterialData.PixelShaderPath)->second;
		Graphics::CMaterial* Material = new Graphics::CMaterial(*PixelShader.get());
		for (size_t i = 0; i < InMaterialData.ImagePaths.size(); ++i)
		{
			auto& ImagePath = InMaterialData.ImagePaths[i];
			if (ImagePath.empty())
				continue;
			auto& Image = Images.find(ImagePath)->second;
			Material->SetImageResource(i, &Image->GetSRV(), &Image->GetTexture2D());
		}

		Materials.emplace(InMaterialData, Material);

		assert(Material);

		return Material;
	}

	std::unique_ptr<Graphics::CBuffer> CreateConstBuffer(size_t InByteWidth) const
	{
		Graphics::TBufferDesc ConstBufferDesc;
		ConstBufferDesc.ByteWidth = uint32_t(InByteWidth);
		ConstBufferDesc.Usage = Graphics::EUsage::UsageDynamic;
		ConstBufferDesc.BindFlags = Graphics::EBindFlags::BindConstantBuffer;
		ConstBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;
		return Device->CreateBuffer(ConstBufferDesc, nullptr);
	}

	std::unique_ptr<Graphics::CBuffer> CreateUAVBuffer(size_t InStructSize, size_t InNumOfElement) const
	{
		Graphics::TBufferDesc UAVBufferDesc;
		UAVBufferDesc.Usage = Graphics::EUsage::UsageDefault;
		UAVBufferDesc.ByteWidth = uint32_t(InStructSize * InNumOfElement);
		UAVBufferDesc.BindFlags = Graphics::EBindFlags::BindUnorderedAccess;
		UAVBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		UAVBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		UAVBufferDesc.StructureByteStride = uint32_t(InStructSize);

		return Device->CreateBuffer(UAVBufferDesc, nullptr);
	}

	std::unique_ptr<Graphics::CBuffer> CreateStagingBuffer(size_t InStructSize, size_t InNumOfElement) const
	{
		Graphics::TBufferDesc StagingBufferDesc;
		StagingBufferDesc.Usage = Graphics::EUsage::UsageStaging;
		StagingBufferDesc.ByteWidth = uint32_t(InStructSize * InNumOfElement);
		StagingBufferDesc.BindFlags = Graphics::EBindFlags::InValid;
		StagingBufferDesc.CPUAccessFlags =
			Graphics::ECPUAccessFlags(uint32_t(Graphics::ECPUAccessFlags::CpuAccessRead) | uint32_t(Graphics::ECPUAccessFlags::CpuAccessWrite));
		StagingBufferDesc.MiscFlags = 0;
		StagingBufferDesc.StructureByteStride = 0;

		return Device->CreateBuffer(StagingBufferDesc, nullptr);
	}

	std::unique_ptr<Graphics::CBuffer> CreateStructuredBuffer(size_t InStructSize, size_t InNumOfElement) const
	{
		Graphics::TBufferDesc BufferDesc;

		BufferDesc.Usage = Graphics::EUsage::UsageDynamic;
		BufferDesc.ByteWidth = uint32_t(InStructSize * InNumOfElement);
		BufferDesc.BindFlags = Graphics::EBindFlags::BindShaderResource;
		BufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;
		BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		BufferDesc.StructureByteStride = uint32_t(InStructSize);

		return Device->CreateBuffer(BufferDesc, nullptr);
	}
	std::unique_ptr<Graphics::CShaderResourceView> CreateStructuredBufferSRV(Graphics::CBuffer& InBuffer, size_t InNumOfElement) const
	{
		Graphics::TShaderResourceViewDesc SRVDesc;

		// 1. [중요] StructuredBuffer는 포맷을 모름 (Unknown)
		SRVDesc.Format = Graphics::EGIFormat::GI_FORMAT_UNKNOWN;

		// 2. 버퍼 뷰
		SRVDesc.ViewDimension = Graphics::EResourceDimension::RESOURCE_DIMENSION_BUFFER;

		// 3. 요소 개수 설정
		SRVDesc.Buffer.FirstElement = 0;
		SRVDesc.Buffer.NumElements = uint32_t(InNumOfElement);

		// 래퍼 클래스에 맞게 생성 호출 (내부적으로 ID3D11Device::CreateShaderResourceView 호출)
		return Device->CreateShaderResourceView(InBuffer, SRVDesc);
	}

	std::unique_ptr<Graphics::CUnorderedAccessView> CreateUAV(const Graphics::CBuffer& InUAVBuffer, size_t InNumOfElement) const
	{
		Graphics::TUnorderedAccessViewDesc UAVDesc;
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.FirstElement = 0;
		UAVDesc.Buffer.NumElements = UINT(InNumOfElement);
		UAVDesc.Buffer.Flags = 0;

		return Device->CreateUnorderedAccessView(InUAVBuffer, UAVDesc);
	}

private:
	Graphics::CRenderDevice* Device;

	std::map<Graphics::MeshKey, std::unique_ptr<Graphics::CMesh>> Meshes;
	std::map<Graphics::TMaterialData, std::unique_ptr<Graphics::CMaterial>> Materials;

	std::map<std::wstring, std::unique_ptr<Graphics::CPixelShader>> PixelShaders;
	std::map<std::wstring, std::unique_ptr<CImage>> Images;

};

