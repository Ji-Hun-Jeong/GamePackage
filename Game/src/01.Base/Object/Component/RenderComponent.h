#pragma once
#include "Component.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
	GENERATE_OBJECT()
public:
	CRenderComponent()
		: RenderStateObject(nullptr)
		, RenderResourceLoader(nullptr)
	{}
	~CRenderComponent()
	{
		
	}

public:
	void EndPlay() override
	{
		CComponent::EndPlay();
		RenderStateObject->Destroy();
	}
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		if (ImagePath.empty() == false)
		{
			CSerializer Data;
			Data["image_path"] = ImagePath.c_str();
			InSerializer["render_component"] = Data;
		}
	}
	void SetMesh(Graphics::MeshKey InMeshKey)
	{
		Graphics::CMesh* Mesh = RenderResourceLoader->GetMeshOrNull(InMeshKey);
		RenderStateObject->SetMesh(Mesh);
	}
	void SetImage(const std::wstring& InImagePath)
	{
		CImage* Image = RenderResourceLoader->LoadImageFromFile(InImagePath);
		RenderStateObject->SetImage(Image);
		ImagePath = InImagePath;
		ImageScale = Image->GetImageScale();
		ImageChangeEvent(ImageScale);
	}
	void SetImage(CImage* InImage)
	{
		RenderStateObject->SetImage(InImage);
		ImageScale = InImage->GetImageScale();
		ImageChangeEvent(ImageScale);
	}
	void SetPSO(EPSOType InPSOType)
	{
		CPSO* PSO = RenderResourceLoader->GetPSO(InPSOType);
		RenderStateObject->SetPSO(PSO);
	}
	void AddVertexConstBuffer(uint32_t InByteWidth)
	{
		Graphics::TBufferDesc ConstBufferDesc;
		ConstBufferDesc.ByteWidth = InByteWidth;
		ConstBufferDesc.Usage = Graphics::EUsage::UsageDynamic;
		ConstBufferDesc.BindFlags = Graphics::EBindFlags::BindConstantBuffer;
		ConstBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;

		std::unique_ptr<Graphics::CBuffer> VertexConstBuffer = RenderResourceLoader->CreateConstBuffer(ConstBufferDesc, nullptr);
		auto VertexConstBufferMapInstance = RenderStateObject->AddVertexConstBuffer(std::move(VertexConstBuffer));
		VertexConstBufferMapInstances.push_back(std::move(VertexConstBufferMapInstance));
	}
	void UpdateVertexConstBuffer(size_t InIndex, const void* InMapDataPoint, size_t InDataSize)
	{
		RenderStateObject->UpdateVertexConstBuffer(VertexConstBufferMapInstances[InIndex].get(), InMapDataPoint, InDataSize);
	}
	const Vector3& GetImageScale() const { return ImageScale; }

public:
	void SetRenderStateObject(CRenderStateObject* InRenderStateObject)
	{
		assert(InRenderStateObject);
		RenderStateObject = InRenderStateObject;
	}
	void SetRenderResourceLoader(CRenderResourceLoader* InRenderResourceLoader)
	{
		assert(InRenderResourceLoader);
		RenderResourceLoader = InRenderResourceLoader;
	}
	void SetImageChangeEvent(std::function<void(const Vector3&)> InImageChangeEvent) { ImageChangeEvent = InImageChangeEvent; }

private:
	Vector3 ImageScale;
	std::wstring ImagePath;
	CRenderStateObject* RenderStateObject;
	CRenderResourceLoader* RenderResourceLoader;

	std::vector<std::unique_ptr<CBufferMapInstance>> VertexConstBufferMapInstances;
	std::function<void(const Vector3&)> ImageChangeEvent;
};

