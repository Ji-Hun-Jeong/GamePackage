#pragma once
#include "Actor.h"

class CStaticActor : public CActor
{
	GENERATE_OBJECT(CStaticActor)
public:
	CStaticActor()
		: CActor()
	{
		RenderComponent = AddComponent<CRenderComponent>();
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		RenderComponent->SetMesh(MeshData);
		RenderComponent->SetPSO(EPSOType::Basic);
		RenderComponent->SetConstBuffer(EShaderType::VertexShader, 0, sizeof(Matrix));
	}

public:
	void FinalUpdate() override
	{
		CActor::FinalUpdate();

		if (RenderComponent->IsExistImage())
		{
			auto CurrentImageDesc = RenderComponent->GetCurrentImageDesc();
			Vector3 Scale{ float(CurrentImageDesc.Width), float(CurrentImageDesc.Height), Transform->GetScale().z };
			Transform->SetScale(Scale);
		}
	}
	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();
		if (Transform->OnVariation())
		{
			uint32_t ScreenWidth = CWindowManager::GetInst().GetScreenWidth();
			uint32_t ScreenHeight = CWindowManager::GetInst().GetScreenHeight();
			const Matrix& NDCModelMatrix = Transform->GetNDCModelMatrix(ScreenWidth, ScreenHeight).Transpose();

			RenderComponent->UpdateConstBuffer(EShaderType::VertexShader, 0, &NDCModelMatrix, sizeof(Matrix));

			Transform->ClearVariation();
		}

	}
};

