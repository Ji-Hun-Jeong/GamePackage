#include "pch.h"
#include "RenderComponent.h"
#include "Transform.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/PSOManager.h"

void CRenderComponent::SetupResource(CRenderResourceLoader& InRenderResourceLoader)
{
	Graphics::CMesh* Mesh = InRenderResourceLoader.LoadMesh(MeshData);
	RenderStateObject.SetMesh(Mesh);
	if (Mesh == nullptr)
		std::cout << "CRenderComponent: Mesh Is None\n";

	CImage* Image = InRenderResourceLoader.LoadImageFromFile(CurrentImagePath);
	RenderStateObject.SetPixelShaderResource(0, Image);
	RenderStateObject.SetPixelShaderResourceStartSlot(0);

	while (MakeVertexConstBufferDataRequestSlots.empty() == false)
	{
		size_t Slot = MakeVertexConstBufferDataRequestSlots.front();
		MakeVertexConstBufferDataRequestSlots.pop();

		auto& VertexConstBufferData = VertexConstBufferDatas[Slot];

		auto VertexConstBuffer = InRenderResourceLoader.CreateConstBuffer(VertexConstBufferData->BufferData.size());
		RenderStateObject.SetVertexConstBuffer(Slot, std::move(VertexConstBuffer));
	}

}

void CRenderComponent::SetupPSO(const CPSOManager& InPSOManager)
{
	CPSO* PSO = InPSOManager.GetPSO(PSOType);
	RenderStateObject.SetPSO(PSO);
}

void CRenderComponent::UpdateModelVertexConstBufferData(const CTransform& InTransform, uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	float NormalizedX = (InTransform.GetFinalPosition().x / (InScreenWidth * 0.5f));
	float NormalizedY = (InTransform.GetFinalPosition().y / (InScreenHeight * 0.5f));

	// 3. Model Matrix ±¸¼º
	Matrix ModelMatrix = (Matrix::CreateScale(InTransform.GetScale())
		* Matrix::CreateRotationX(InTransform.GetRotation().x)
		* Matrix::CreateRotationY(InTransform.GetRotation().y)
		* Matrix::CreateRotationZ(InTransform.GetRotation().z)
		* Matrix::CreateTranslation(NormalizedX, NormalizedY, InTransform.GetFinalPosition().z)).Transpose();

	UpdateVertexConstBufferData(0, &ModelMatrix, sizeof(ModelMatrix));
}

