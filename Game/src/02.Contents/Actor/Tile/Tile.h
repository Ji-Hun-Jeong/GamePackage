#pragma once
#include "01.Base/Actor/StaticActor.h"

enum class ETilePutMode
{
	Center,
	Up,
	Down,
	Left,
	Right,
};
class CTile : public CStaticActor
{
	GENERATE_OBJECT(CTile)
public:
	CTile()
	{
		InteractionComponent = AddComponent<CInteractionComponent>();
		SpriteRenderComponent->SetMesh(CAssetLoader::GetInst().GetMeshData("RectMesh"));
		SpriteRenderComponent->SetPSO(EPSOType::Rect);
		SpriteRenderComponent->SetLayer(1);
	}
	~CTile() = default;

public:
	void CaptureSnapShot() override
	{
		CStaticActor::CaptureSnapShot();

		const Vector3& FinalPosition = Transform->GetFinalPosition();
		const Vector3& Scale = Transform->GetScale();
		InteractionComponent->SetRectPosition(FinalPosition.x, FinalPosition.y);
		InteractionComponent->SetRectScale(Scale.x, Scale.y);
	}
	void PutOnActor(const std::wstring& InActorImagePath, ETilePutMode InTilePutMode = ETilePutMode::Center);
	void RevertPutOn();

private:
	CStaticActor* ManagingActor = nullptr;

};

