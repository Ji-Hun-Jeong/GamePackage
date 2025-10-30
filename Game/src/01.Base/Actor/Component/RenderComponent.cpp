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

void CRenderComponent::UpdateModelDataToNDC(const CTransform& InTransform, uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	const Vector3& scale = InTransform.GetScale();
	Vector3 normalizedScale = Vector3(
		scale.x / InScreenWidth,
		scale.y / InScreenHeight,
		scale.z // Z축은 보통 그대로 둠
	);
	float NormalizedX = (InTransform.GetFinalPosition().x / (InScreenWidth * 0.5f));
	float NormalizedY = (InTransform.GetFinalPosition().y / (InScreenHeight * 0.5f));

	Matrix ModelMatrix = (Matrix::CreateScale(normalizedScale)
		* Matrix::CreateRotationX(InTransform.GetRotation().x)
		* Matrix::CreateRotationY(InTransform.GetRotation().y)
		* Matrix::CreateRotationZ(InTransform.GetRotation().z)
		* Matrix::CreateTranslation(NormalizedX, NormalizedY, InTransform.GetFinalPosition().z)).Transpose();

	UpdateVertexConstBufferData(0, &ModelMatrix, sizeof(ModelMatrix));
}

void CRenderComponent::SynchronizeScaleToImageOnImageChange(CTransform& InTransform)
{
	if (bUpdateImage)
	{
		Vector3 Scale{ float(CurrentImageDesc.Width), float(CurrentImageDesc.Height), InTransform.GetScale().z };
		InTransform.SetScale(Scale);
	}
}

