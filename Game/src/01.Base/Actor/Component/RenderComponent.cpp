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

	ModelMatrix = (Matrix::CreateScale(Scale)
		* Matrix::CreateRotationX(Rotation.x)
		* Matrix::CreateRotationY(Rotation.y)
		* Matrix::CreateRotationZ(Rotation.z)
		* Matrix::CreateTranslation(Position)).Transpose();
}

void CRenderComponent::SetupPSO(const CPSOManager& InPSOManager)
{
	CPSO* PSO = InPSOManager.GetPSO(PSOType);
	RenderStateObject.SetPSO(PSO);
}

