#pragma once
#include "Actor.h"
#include "Component/RenderComponents/SpriteRenderComponent.h"

class CStaticActor : public CActor
{
	GENERATE_OBJECT(CStaticActor)
public:
	CStaticActor()
		: CActor()
	{
		SpriteRenderComponent = AddComponent<CSpriteRenderComponent>();

		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("SquareMesh");
		SpriteRenderComponent->SetMesh(MeshData);
	}

public:
	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();
	}
	void SetLineActor()
	{
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
		SpriteRenderComponent->SetMesh(MeshData);
		SpriteRenderComponent->SetPSO(EPSOType::Line);
		SpriteRenderComponent->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	}

};

