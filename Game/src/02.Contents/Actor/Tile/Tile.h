#pragma once
#include "01.Base/Actor/StaticActor.h"

enum class ETilePositionType
{
	Center,
	Left,
	Right,
	Top,
	Bottom,
	LeftTop,
	LeftBottom,
	RightTop,
	RightBottom,
	None,
};

class CTile : public CStaticActor
{
	GENERATE_OBJECT(CTile)
public:
	CTile()
	{
		SpriteRenderComponent->SetMesh(CAssetLoader::GetInst().GetMeshData("ImageMesh"));
		SpriteRenderComponent->SetLayer(1);
	}
	~CTile() = default;

public:
	void CaptureSnapShot() override
	{
		CStaticActor::CaptureSnapShot();
		SpriteRenderComponent->SetEdge(Vector3(0.0f, 0.0f, 0.0f), 2, Transform->GetScale().x, Transform->GetScale().y);
	}
	void MoveActor(CStaticActor& InMovedActor, ETilePositionType InTilePositionType);
	void ChangeEdge(const Vector3& InEdgeColor)
	{
		SpriteRenderComponent->SetEdge(InEdgeColor, 2
			, Transform->GetScale().x, Transform->GetScale().y);
	}
	void RevertEdge()
	{
		SpriteRenderComponent->SetEdge(Vector3(0.0f, 0.0f, 0.0f), 2, Transform->GetScale().x, Transform->GetScale().y);
	}

};

