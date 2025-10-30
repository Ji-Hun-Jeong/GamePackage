#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CCpuBuffer
{
	friend class CRenderComponent;
public:
	CCpuBuffer(const void* InMappingPoint, size_t InByteWidth)
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
		: PSOType(EPSOType::Basic)
	{}
	~CRenderComponent()
	{
	}

public:
	void SetMesh(const Graphics::TMeshData& InMeshData)
	{
		MeshData = InMeshData;
	}
	void SetDiffuseImage(const std::wstring& InImagePath)
	{
		CurrentImagePath = InImagePath;
	}
	void SetPSO(EPSOType InPSOType)
	{
		PSOType = InPSOType;
	}

	CRenderStateObject* GetRenderStateObject() { return &RenderStateObject; }
	void SetupResourceToRSO(class CRenderResourceLoader& InRenderResourceLoader);
	void SetupMappingInstanceToRSO(class CRenderResourceLoader& InRenderResourceLoader);
	void SetupPSOToRSO(const class CPSOManager& InPSOManager);

	void SetVertexConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		if (VertexConstBufferDatas[InSlot])
		{
			VertexConstBufferDatas[InSlot].reset();
			std::cout << "ConstBuffer Covered\n";
		}
		VertexConstBufferDatas[InSlot] = std::make_unique<CCpuBuffer>(InMappingPoint, InByteWidth);
	}

	void UpdateVertexConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		if (VertexConstBufferDatas[InSlot] == nullptr)
			SetVertexConstBufferData(InSlot, InMappingPoint, InByteWidth);
		
		memcpy(VertexConstBufferDatas[InSlot]->BufferData.data(), InMappingPoint, InByteWidth);
		VertexConstBufferDatas[InSlot]->bUpdated = true;
	}

	void MapBuffersToRSO()
	{
		for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
		{
			auto& CpuBuffer = VertexConstBufferDatas[i];
			if (CpuBuffer == nullptr)
				continue;
			if (CpuBuffer->bUpdated == false)
				continue;

			RenderStateObject.UpdateMappingInstance(i, CpuBuffer->BufferData.data(), CpuBuffer->BufferData.size());

			CpuBuffer->bUpdated = false;
		}
	}

	void UpdateModelDataToNDC(const class CTransform& InTransform, uint32_t InScreenWidth, uint32_t InScreenHeight);

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

	std::array<std::unique_ptr<CCpuBuffer>, 6> VertexConstBufferDatas;

};

