#pragma once
#include "Component.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/RenderStateObject.h"
#include "04.Renderer/SpriteRenderer.h"

class CRenderComponent : public CComponent
{
	GENERATE_OBJECT()
public:
	CRenderComponent()
		: RenderStateObject(nullptr)
		, RenderResourceLoader(nullptr)
		, Renderer(nullptr)
		, CurrentSettingImage(nullptr)
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
		if (CurrentSettingImagePath.empty() == false)
		{
			CSerializer Data;
			Data["Name"] = "CRenderComponent";
			Data["ImagePath"] = CurrentSettingImagePath.c_str();
			InSerializer = Data;
		}
	}
	void SetMesh(Graphics::MeshKey InMeshKey)
	{
		Graphics::CMesh* Mesh = RenderResourceLoader->GetMeshOrNull(InMeshKey);
		RenderStateObject->SetMesh(Mesh);
	}
	void SetDiffuseImage(const std::wstring& InImagePath)
	{
		CImage* Image = nullptr;
		auto Iter = CacheImages.find(InImagePath);
		if (Iter != CacheImages.end())
			Image = Iter->second;
		else
		{
			Image = RenderResourceLoader->LoadImageFromFile(InImagePath);
			CacheImages.emplace(InImagePath, Image);
		}
		CurrentSettingImagePath = InImagePath;
		CurrentSettingImage = Image;
		assert(CurrentSettingImage);

		RenderStateObject->SetPixelShaderResource(0, CurrentSettingImage);

		const Vector2& ImageSize = CurrentSettingImage->GetTexture2D().GetTextureSize();
		for (auto& ImageChangeEvent : ImageChangeEvents)
			ImageChangeEvent(ImageSize);
	}
	void SetPSO(EPSOType InPSOType)
	{
		CPSO* PSO = RenderResourceLoader->GetPSO(InPSOType);
		RenderStateObject->SetPSO(PSO);
	}
	void ResetImage()
	{
		CurrentSettingImagePath = L"";
		CurrentSettingImage = nullptr;
		RenderStateObject->SetPixelShaderResource(0, CurrentSettingImage);

		Vector2 ImageSize(0.0f);
		for (auto& ImageChangeEvent : ImageChangeEvents)
			ImageChangeEvent(ImageSize);
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
	const Vector2& GetCurrentImageScale() const { return CurrentSettingImage->GetTexture2D().GetTextureSize(); }

	Matrix GetModelMatrix(const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale)
	{
		Vector3 Position = Vector3(InPosition.x * 2.0f / Renderer->GetScreenWidth(), InPosition.y * 2.0f / Renderer->GetScreenHeight(), InPosition.z);
		const Vector3& Rotation = InRotation;
		Vector3 Scale = Vector3(InScale.x / Renderer->GetScreenWidth(), InScale.y / Renderer->GetScreenHeight(), InScale.z);

		return (Matrix::CreateScale(Scale)
			* Matrix::CreateRotationX(Rotation.x)
			* Matrix::CreateRotationY(Rotation.y)
			* Matrix::CreateRotationZ(Rotation.z)
			* Matrix::CreateTranslation(Position)).Transpose();
	}
	void AddImageChangeEvent(std::function<void(const Vector2&)> InImageChangeEvent)
	{
		ImageChangeEvents.push_back(InImageChangeEvent);
	}
	const std::wstring& GetCurrentImagePath() const { return CurrentSettingImagePath; }

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
	void SetRenderer(const CSpriteRenderer* InRenderer) { Renderer = InRenderer; }

private:
	const CSpriteRenderer* Renderer;
	CRenderStateObject* RenderStateObject;
	CRenderResourceLoader* RenderResourceLoader;

	std::vector<std::unique_ptr<CBufferMapInstance>> VertexConstBufferMapInstances;

	std::vector<std::function<void(const Vector2&)>> ImageChangeEvents;

	std::map<std::wstring, CImage*> CacheImages;
	CImage* CurrentSettingImage;
	std::wstring CurrentSettingImagePath;

};

