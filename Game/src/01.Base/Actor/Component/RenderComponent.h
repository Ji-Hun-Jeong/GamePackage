#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CCpuBuffer
{
	friend class CRenderComponent;
public:
	CCpuBuffer(const void* InMappingPoint, size_t InByteWidth)
	{
		BufferData.resize(InByteWidth, 0);
		memcpy(BufferData.data(), InMappingPoint, InByteWidth);
	}

private:
	std::vector<uint8_t> BufferData;

};

class CRenderComponent : public CComponent
{
	GENERATE_OBJECT(CRenderComponent)
public:
	CRenderComponent()
		: PSOType(EPSOType::Basic)
		, Scale(1.0f)
	{
		SetVertexConstBufferData(0, &ModelMatrix, sizeof(ModelMatrix));
	}
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
	std::queue<TBufferMappingInstance>& GetUpdateBufferList() { return UpdateBufferList; }

	void SetupResource(class CRenderResourceLoader& InRenderResourceLoader);
	void SetupPSO(const class CPSOManager& InPSOManager);

	void SetVertexConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		if (VertexConstBufferDatas[InSlot])
		{
			VertexConstBufferDatas[InSlot].reset();
			std::cout << "ConstBuffer Covered\n";
		}
		VertexConstBufferDatas[InSlot] = std::make_unique<CCpuBuffer>(InMappingPoint, InByteWidth);
		MakeVertexConstBufferDataRequestSlots.push(InSlot);
	}
	void UpdateVertexConstBufferData(size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		if (VertexConstBufferDatas[InSlot] == nullptr)
			SetVertexConstBufferData(InSlot, InMappingPoint, InByteWidth);
		
		memcpy(VertexConstBufferDatas[InSlot]->BufferData.data(), InMappingPoint, InByteWidth);
		UpdateBufferList.emplace(RenderStateObject, InSlot, VertexConstBufferDatas[InSlot]->BufferData);
	}

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
	std::queue<size_t> MakeVertexConstBufferDataRequestSlots;

	std::queue<TBufferMappingInstance> UpdateBufferList;

public:
	void SetPosition(const Vector3& InPosition)
	{
		Position = InPosition;
	}
	void SetRotation(const Vector3& InRotation)
	{
		Rotation = InRotation;
	}
	void SetScale(const Vector3& InScale)
	{
		Scale = InScale;
	}

private:
	Matrix ModelMatrix;
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

};

