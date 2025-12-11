#pragma once
#include "01.Base/Actor/UI.h"

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

class CTile : public CUI
{
	GENERATE_OBJECT(CTile)
public:
	CTile()
		: CUI()
	{
		static const Graphics::TMeshData& LineSquareMeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
		SpriteRenderComponent->SetLayer(1);
		SpriteRenderComponent->SetMesh(LineSquareMeshData);
		SpriteRenderComponent->SetPSO(EPSOType::Line);
		SpriteRenderComponent->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	}
	~CTile() = default;

public:
	void CaptureSnapShot() override
	{
		CStaticActor::CaptureSnapShot();
		//SpriteRenderComponent->SetEdge(Vector3(0.0f, 0.0f, 0.0f), 2, Transform->GetScale().x, Transform->GetScale().y);
	}
	void MoveActor(CStaticActor& InMovedActor, ETilePositionType InTilePositionType);
	void ChangeEdge(const Vector3& InEdgeColor)
	{
		SpriteRenderComponent->SetColor(InEdgeColor, 1.0f);
	}
	void RevertEdge()
	{
		SpriteRenderComponent->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	}

};

