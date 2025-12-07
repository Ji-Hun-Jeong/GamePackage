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

		if (InteractionComponent && SpriteRenderComponent->IsImageType())
		{
			const Vector2& ImageScale = SpriteRenderComponent->GetImageScale();
			InteractionComponent->SetRectScale(ImageScale.x, ImageScale.y);
		}
	}
	CSpriteRenderComponent* GetSpriteRenderComponent() const { return SpriteRenderComponent; }

protected:
	CSpriteRenderComponent* SpriteRenderComponent;

};

