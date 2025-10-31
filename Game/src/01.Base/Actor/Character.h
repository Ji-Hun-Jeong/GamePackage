#pragma once
#include "Actor.h"

class CCharacter : public CActor
{
	GENERATE_OBJECT(CCharacter)
	DONTCOPY(CCharacter)
public:
	CCharacter()
	{
		RenderComponent = AddComponent<CRenderComponent>();
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		RenderComponent->SetMesh(MeshData);
	}
	virtual ~CCharacter() {}
public:
	//virtual void Initalize() override
	//{
	//	CActor::Initalize();
	//	//CActor::SetRenderComponent();
	//}

private:

};

