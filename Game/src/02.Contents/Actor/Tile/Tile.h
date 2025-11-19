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
		SpriteRenderComponent->SetMesh(CAssetLoader::GetInst().GetMeshData("RectMesh"));
		SpriteRenderComponent->SetPSO(EPSOType::Rect);
		SpriteRenderComponent->SetLayer(1);
	}
	~CTile() = default;

public:
	void SetPutOnActor(CStaticActor* InPutOnActor)
	{
		PutOnActor = InPutOnActor;
	}
	CStaticActor* GetPutOnActor() const { return PutOnActor; }
	void MoveActor(ETilePositionType InTilePositionType);

private:
	CStaticActor* PutOnActor = nullptr;

};

