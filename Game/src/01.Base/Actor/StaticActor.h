#pragma once
#include "Actor.h"

class CStaticActor : public CActor
{
	GENERATE_OBJECT(CStaticActor)
public:
	CStaticActor()
	{
		RenderComponent = AddComponent<CRenderComponent>();
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		RenderComponent->SetMesh(MeshData);
	}
private:

};

