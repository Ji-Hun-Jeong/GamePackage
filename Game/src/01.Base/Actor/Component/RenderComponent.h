#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CCpuBuffer
{
	friend class CRenderComponent;
public:
	CCpuBuffer(size_t InByteWidth)
		: bUpdated(true)
	{
		BufferData.resize(InByteWidth, 0);
	}

private:
	std::vector<uint8_t> BufferData;

	bool bUpdated;
};

class CRenderComponent : public CComponent
{
	GENERATE_OBJECT(CRenderComponent)
public:
	CRenderComponent()
		: PSOType(EPSOType::End)
		, bUpdateMesh(false)
		, bUpdateImage(false)
		, bUpdatePSO(false)
	{
		SetVertexConstBufferData(0, sizeof(Matrix));
		SetPSO(EPSOType::Basic);
	}
	~CRenderComponent()
	{
	}

public:
	void SetMesh(const Graphics::TMeshData& InMeshData)
	{
		MeshData = InMeshData;
		bUpdateMesh = true;
	}
	void SetDiffuseImage(const std::wstring& InImagePath)
	{
		CurrentImagePath = InImagePath;
		bUpdateImage = true;
	}
	void SetPSO(EPSOType InPSOType)
	{
		PSOType = InPSOType;
		bUpdatePSO = true;
	}
	void SetLayer(uint32_t InLayer) { RenderStateObject.SetRenderLayer(InLayer); }

	CRenderStateObject* GetRenderStateObject() { return &RenderStateObject; }
	void SetupResourceToRSO(class CRenderResourceLoader& InRenderResourceLoader);
	void SetupMappingInstanceToRSO(class CRenderResourceLoader& InRenderResourceLoader);
	void SetupPSOToRSO(const class CPSOManager& InPSOManager);

	void SetVertexConstBufferData(size_t InSlot, size_t InByteWidth)
	{
		if (VertexConstBufferDatas[InSlot])
		{
			VertexConstBufferDatas[InSlot].reset();
			std::cout << "ConstBuffer Covered\n";
		}
		VertexConstBufferDatas[InSlot] = std::make_unique<CCpuBuffer>(InByteWidth);
		VertexConstBufferDatas[InSlot]->bUpdated = true;
	}

	void UpdateVertexConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		memcpy(VertexConstBufferDatas[InSlot]->BufferData.data(), InMappingPoint, InByteWidth);
		VertexConstBufferDatas[InSlot]->bUpdated = true;
	}

	void SetPixelConstBufferData(size_t InSlot, size_t InByteWidth)
	{
		if (PixelConstBufferDatas[InSlot])
		{
			PixelConstBufferDatas[InSlot].reset();
			std::cout << "ConstBuffer Covered\n";
		}
		PixelConstBufferDatas[InSlot] = std::make_unique<CCpuBuffer>(InByteWidth);
		PixelConstBufferDatas[InSlot]->bUpdated = true;
	}

	void UpdatePixelConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		memcpy(PixelConstBufferDatas[InSlot]->BufferData.data(), InMappingPoint, InByteWidth);
		PixelConstBufferDatas[InSlot]->bUpdated = true;
	}

	void MapUpdatedBuffersToRSO()
	{
		for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
		{
			auto& CpuBuffer = VertexConstBufferDatas[i];
			if (CpuBuffer == nullptr)
				continue;
			if (CpuBuffer->bUpdated == false)
				continue;

			RenderStateObject.UpdateVertexConstMappingInstance(i, CpuBuffer->BufferData.data(), CpuBuffer->BufferData.size());

			CpuBuffer->bUpdated = false;
		}
		for (size_t i = 0; i < PixelConstBufferDatas.size(); ++i)
		{
			auto& CpuBuffer = PixelConstBufferDatas[i];
			if (CpuBuffer == nullptr)
				continue;
			if (CpuBuffer->bUpdated == false)
				continue;

			RenderStateObject.UpdatePixelConstMappingInstance(i, CpuBuffer->BufferData.data(), CpuBuffer->BufferData.size());

			CpuBuffer->bUpdated = false;
		}
	}

	void ClearState()
	{
		bUpdateMesh = false;
		bUpdateImage = false;
		bUpdatePSO = false;
		for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
		{
			if (VertexConstBufferDatas[i])
				VertexConstBufferDatas[i]->bUpdated = false;
		}
		for (size_t i = 0; i < PixelConstBufferDatas.size(); ++i)
		{
			if (PixelConstBufferDatas[i])
				PixelConstBufferDatas[i]->bUpdated = false;
		}
	}

	bool IsUpdateImage() const { return bUpdateImage; }
	const Graphics::TTexture2DDesc& GetCurrentImageDesc() const { return CurrentImageDesc; }

public:
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		if (CurrentImagePath.empty() == false)
		{
			CSerializer Data;
			Data["ImagePath"] = CurrentImagePath.c_str();
			InSerializer = Data;
		}
	}
	void Deserialize(const CSerializer& InDeserializer) override
	{
		CComponent::Deserialize(InDeserializer);
		const std::string& Path = InDeserializer["ImagePath"];
		CurrentImagePath = std::wstring(Path.begin(), Path.end());
		SetDiffuseImage(CurrentImagePath);
	}

private:
	CRenderStateObject RenderStateObject;

	Graphics::TMeshData MeshData;
	EPSOType PSOType;
	std::wstring CurrentImagePath;
	Graphics::TTexture2DDesc CurrentImageDesc;

	std::array<std::unique_ptr<CCpuBuffer>, 6> VertexConstBufferDatas;
	std::array<std::unique_ptr<CCpuBuffer>, 6> PixelConstBufferDatas;

	bool bUpdateMesh;
	bool bUpdateImage;
	bool bUpdatePSO;

};

