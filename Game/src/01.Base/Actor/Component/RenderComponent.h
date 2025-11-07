#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"
#include "04.Renderer/SpriteRenderer.h"

class CCpuBuffer
{
public:
	CCpuBuffer()
		: bUpdated(false)
	{}
	~CCpuBuffer() = default;

public:
	void Resize(size_t InSize)
	{
		BufferData.resize(InSize, 0);
	}
	void Copy(const void* InSrc, size_t InSize = 0)
	{
		if (InSize == 0)
			Resize(InSize);

		memcpy(BufferData.data(), InSrc, BufferData.size());
		bUpdated = true;
	}
	void SetUpdate(bool bInUpdated) { bUpdated = bInUpdated; }
	const void* GetBufferData() const { return BufferData.data(); }
	size_t GetBufferSize() const { return BufferData.size(); }
	bool IsUpdated() const { return bUpdated; }

private:
	std::vector<uint8_t> BufferData;

	bool bUpdated;
};

class CImageDesc
{
public:
	CImageDesc()
		: bUpdated(false)
	{}
	~CImageDesc() = default;

public:
	void SetImagePath(const std::wstring& InImagePath)
	{
		ImagePath = InImagePath;
		bUpdated = true;
	}
	void SetImageDesc(const Graphics::TTexture2DDesc& InImageTextureDesc)
	{
		ImageTextureDesc = InImageTextureDesc;
	}
	void SetUpdate(bool bInUpdated) { bUpdated = bInUpdated; }
	const std::wstring& GetImagePath() const { return ImagePath; }
	const Graphics::TTexture2DDesc& GetImageTextureDesc() const { return ImageTextureDesc; }
	bool IsValid() const { return ImagePath.size(); }
	bool IsUpdated() const { return bUpdated; }

private:
	std::wstring ImagePath;
	Graphics::TTexture2DDesc ImageTextureDesc;
	bool bUpdated;

};

class CRenderComponent : public CComponent
{
	GENERATE_OBJECT(CRenderComponent)
public:
	CRenderComponent()
		: PSOType(EPSOType::End)
		, bUpdateMesh(false)
		, bUpdatePSO(false)
	{}
	~CRenderComponent()
	{}

public:
	void SetMesh(const Graphics::TMeshData& InMeshData)
	{
		MeshData = InMeshData;
		bUpdateMesh = true;
	}
	void SetDiffuseImage(const std::wstring& InImagePath)
	{
		ImageDescs[0].SetImagePath(InImagePath);
	}
	void SetPSO(EPSOType InPSOType)
	{
		PSOType = InPSOType;
		bUpdatePSO = true;
	}
	void SetLayer(uint32_t InLayer) { RenderStateObject.SetRenderLayer(InLayer); }

	void SetConstBuffer(EShaderType InShaderType, size_t InSlot, size_t InByteWidth)
	{
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			VertexConstBufferDatas[InSlot].Resize(InByteWidth);
			break;
		case EShaderType::PixelShader:
			PixelConstBufferDatas[InSlot].Resize(InByteWidth);
			break;
		default:
			assert(0);
			break;
		}
	}
	void UpdateConstBuffer(EShaderType InShaderType, size_t InSlot, const void* InMappingPoint, size_t InByteWidth)
	{
		switch (InShaderType)
		{
		case EShaderType::VertexShader:
			VertexConstBufferDatas[InSlot].Copy(InMappingPoint, InByteWidth);
			break;
		case EShaderType::PixelShader:
			PixelConstBufferDatas[InSlot].Copy(InMappingPoint, InByteWidth);
			break;
		default:
			assert(0);
			break;
		}
	}

	void Render(CSpriteRenderer& InRenderer)
	{
		for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
		{
			auto& CpuBuffer = VertexConstBufferDatas[i];
			if (CpuBuffer.IsUpdated() == false)
				continue;
			InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::VertexShader, i, CpuBuffer.GetBufferData(), CpuBuffer.GetBufferSize());
		}
		for (size_t i = 0; i < PixelConstBufferDatas.size(); ++i)
		{
			auto& CpuBuffer = PixelConstBufferDatas[i];
			if (CpuBuffer.IsUpdated() == false)
				continue;
			InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::PixelShader, i, CpuBuffer.GetBufferData(), CpuBuffer.GetBufferSize());
		}

		if (bUpdateMesh)
			InRenderer.SetMeshToRSO(RenderStateObject, MeshData);
		if(bUpdatePSO)
			InRenderer.SetPSOToRSO(RenderStateObject, PSOType);
		for (size_t i = 0; i < ImageDescs.size(); ++i)
		{
			auto& ImageDesc = ImageDescs[i];
			if (ImageDesc.IsUpdated() == false)
				continue;
			CImage* Image = InRenderer.SetShaderResourceToRSOByImagePath(RenderStateObject, ImageDesc.GetImagePath());
			ImageDesc.SetImageDesc(Image->GetTexture2D().GetTexture2DDesc());
		}

		InRenderer.RenderObject(RenderStateObject);

		ClearState();
	}

	const Graphics::TTexture2DDesc& GetCurrentImageDesc() const { return ImageDescs[0].GetImageTextureDesc(); }

private:
	void ClearState()
	{
		bUpdateMesh = false;
		bUpdatePSO = false;
		for (size_t i = 0; i < ImageDescs.size(); ++i)
			ImageDescs[i].SetUpdate(false);
		
		for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
			VertexConstBufferDatas[i].SetUpdate(false);
		
		for (size_t i = 0; i < PixelConstBufferDatas.size(); ++i)
			PixelConstBufferDatas[i].SetUpdate(false);
		
	}

public:
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		if (ImageDescs[0].IsValid())
		{
			CSerializer Data;
			Data["ImagePath"] = ImageDescs[0].GetImagePath().c_str();
			InSerializer = Data;
		}
	}
	void Deserialize(const CSerializer& InDeserializer) override
	{
		CComponent::Deserialize(InDeserializer);
		const std::string& Path = InDeserializer["ImagePath"];
		SetDiffuseImage(std::wstring(Path.begin(), Path.end()));
	}

private:
	CRenderStateObject RenderStateObject;

	Graphics::TMeshData MeshData;
	EPSOType PSOType;
	
	std::array<CImageDesc, 6> ImageDescs;

	std::array<CCpuBuffer, 6> VertexConstBufferDatas;
	std::array<CCpuBuffer, 6> PixelConstBufferDatas;

	bool bUpdateMesh;
	bool bUpdatePSO;

};

