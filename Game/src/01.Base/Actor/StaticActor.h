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
		AddComponent<CSpriteRenderComponent>();

		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("SquareMesh");
		SpriteRenderComponent->SetMesh(MeshData);
	}

public:
	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();
	}


};

