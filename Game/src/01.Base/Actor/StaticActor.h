#pragma once
#include "Actor.h"
#include "Component/RenderComponents/SpriteRenderComponent.h"

class CStaticActor : public CActor
{
	GENERATE_OBJECT(CStaticActor)
public:
	CStaticActor()
		: CActor()
		, SpriteRenderComponent(nullptr)
	{
		SpriteRenderComponent = AddComponent<CSpriteRenderComponent>();
		RenderComponent = SpriteRenderComponent;

		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("SquareMesh");
		SpriteRenderComponent->SetMesh(MeshData);
	}

public:
	void FinalUpdate() override
	{
		CActor::FinalUpdate();
	}
	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();

	}
	void SetLineActor()
	{
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
		SpriteRenderComponent->SetMesh(MeshData);
		SpriteRenderComponent->SetPSO(EPSOType::Line);
	}
	CSpriteRenderComponent* GetSpriteRenderComponent() const { return SpriteRenderComponent; }

protected:
	CSpriteRenderComponent* SpriteRenderComponent;

};

