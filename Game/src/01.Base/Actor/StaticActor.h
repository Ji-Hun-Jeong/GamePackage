#pragma once
#include "Actor.h"

class CStaticActor : public CActor
{
	GENERATE_OBJECT(CStaticActor)
public:
	CStaticActor()
	{
		RenderComponent = AddComponent<CRenderComponent>();
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		RenderComponent->SetMesh(MeshData);
		RenderComponent->SetPSO(EPSOType::Basic);
		RenderComponent->SetConstBuffer(EShaderType::VertexShader, 0, sizeof(Matrix));
	}

public:
	void CaptureSnapShot(uint32_t InScreenWidth, uint32_t InScreenHeight) override
	{
		CActor::CaptureSnapShot(InScreenWidth, InScreenHeight);

		auto CurrentImageDesc = RenderComponent->GetCurrentImageDesc();
		Vector3 Scale{ float(CurrentImageDesc.Width), float(CurrentImageDesc.Height), Transform->GetScale().z };
		Transform->SetScale(Scale);

		const Matrix& NDCModelMatrix = Transform->GetNDCModelMatrix(InScreenWidth, InScreenHeight).Transpose();
		RenderComponent->UpdateConstBuffer(EShaderType::VertexShader, 0, &NDCModelMatrix, sizeof(Matrix));
	}
private:

};

