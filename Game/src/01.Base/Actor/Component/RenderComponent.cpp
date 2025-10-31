#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/PSOManager.h"

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
		CImage* Image = InRenderResourceLoader.LoadImageFromFile(CurrentImagePath);
		RenderStateObject.SetPixelShaderResource(0, Image);
		CurrentImageDesc = Image->GetTexture2D().GetTexture2DDesc();
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
		if (RenderStateObject.IsExistBufferInSlot(i))
			continue;

		size_t ByteWidth = VertexConstBufferData->BufferData.size();
		auto GpuBuffer = InRenderResourceLoader.CreateConstBuffer(ByteWidth);
		RenderStateObject.SetVertexConstBuffer(i, std::make_unique<CBufferMappingInstance>(ByteWidth, std::move(GpuBuffer)));
	}
}

void CRenderComponent::SetupPSOToRSO(const CPSOManager& InPSOManager)
{
	CPSO* PSO = InPSOManager.GetPSO(PSOType);
	RenderStateObject.SetPSO(PSO);
}
