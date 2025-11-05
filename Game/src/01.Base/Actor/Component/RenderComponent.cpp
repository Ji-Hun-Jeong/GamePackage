#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/PSOManager.h"

void CRenderComponent::SetupPSOToRSO(const CPSOManager& InPSOManager)
{
	if (bUpdatePSO)
	{
		CPSO* PSO = InPSOManager.GetPSO(PSOType);
		RenderStateObject.SetPSO(PSO);
	}
}

void CRenderComponent::SetupResourceToRSO(CRenderResourceLoader& InRenderResourceLoader)
{
	if (bUpdateMesh)
	{
		Graphics::CMesh* Mesh = InRenderResourceLoader.LoadMesh(MeshData);
		if (Mesh == nullptr)
			std::cout << "CRenderComponent: Mesh Is None\n";
		RenderStateObject.SetMesh(Mesh);
	}
	if (bUpdateImage)
	{
		CImage* Image = nullptr;
		if (CurrentImagePath.empty() == false)
		{
			Image = InRenderResourceLoader.LoadImageFromFile(CurrentImagePath);
			CurrentImageDesc = Image->GetTexture2D().GetTexture2DDesc();
		}
		else
			ZeroMemory(&CurrentImageDesc, sizeof(CurrentImageDesc));

		RenderStateObject.SetPixelShaderResource(0, Image);
	}
}

void CRenderComponent::SetupMappingInstanceToRSO(CRenderResourceLoader& InRenderResourceLoader)
{
	for (size_t i = 0; i < VertexConstBufferDatas.size(); ++i)
	{
		auto& VertexConstBufferData = VertexConstBufferDatas[i];
		if (VertexConstBufferData == nullptr)
			continue;
		if (VertexConstBufferData->bUpdated == false)
			continue;
		if (RenderStateObject.IsExistVertexConstBufferInSlot(i))
			continue;

		size_t ByteWidth = VertexConstBufferData->BufferData.size();
		auto GpuBuffer = InRenderResourceLoader.CreateConstBuffer(ByteWidth);
		RenderStateObject.SetVertexConstBuffer(i, std::make_unique<CBufferMappingInstance>(ByteWidth, std::move(GpuBuffer)));
	}

	for (size_t i = 0; i < PixelConstBufferDatas.size(); ++i)
	{
		auto& PixelConstBufferData = PixelConstBufferDatas[i];
		if (PixelConstBufferData == nullptr)
			continue;
		if (PixelConstBufferData->bUpdated == false)
			continue;
		if (RenderStateObject.IsExistPixelConstBufferInSlot(i))
			continue;

		size_t ByteWidth = PixelConstBufferData->BufferData.size();
		auto GpuBuffer = InRenderResourceLoader.CreateConstBuffer(ByteWidth);
		RenderStateObject.SetPixelConstBuffer(i, std::make_unique<CBufferMappingInstance>(ByteWidth, std::move(GpuBuffer)));
	}
}