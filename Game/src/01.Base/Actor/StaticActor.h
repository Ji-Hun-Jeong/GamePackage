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

		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		SpriteRenderComponent->SetMesh(MeshData);
		SpriteRenderComponent->SetPSO(EPSOType::Basic);
	}

public:
	void FinalUpdate() override
	{
		CActor::FinalUpdate();
	}
	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();
		if (Transform->OnVariation())
		{
			uint32_t ScreenWidth = CWindowManager::GetInst().GetScreenWidth();
			uint32_t ScreenHeight = CWindowManager::GetInst().GetScreenHeight();
			const Vector3& Position = Transform->GetFinalPosition();
			const Vector3& Rotation = Transform->GetRotation();
			const Vector3& Scale = Transform->GetScale();

			SpriteRenderComponent->UpdateModelToNDC(Position, Rotation, Scale, ScreenWidth, ScreenHeight);

			OnTransformVariation();

			Transform->ClearVariation();
		}
		if (InteractionComponent && SpriteRenderComponent->IsImageType())
		{
			const Vector2& ImageScale = SpriteRenderComponent->GetImageScale();
			InteractionComponent->SetRectScale(ImageScale.x, ImageScale.y);
		}
	}
	virtual void OnTransformVariation() {}
	CSpriteRenderComponent* GetSpriteRenderComponent() const { return SpriteRenderComponent; }

protected:
	CSpriteRenderComponent* SpriteRenderComponent;

};

