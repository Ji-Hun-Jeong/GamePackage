#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"

class CTestActor : public CActor 
{
	GENERATE_OBJECT(CTestActor)
public:
	CTestActor()
	{
		RenderComponent = AddComponent<CRenderComponent>();
		
		const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("ImageMesh");
		RenderComponent->SetMesh(MeshData);
		RenderComponent->SetDiffuseImage(L"resources/image/UI/Title/Background.png");
		RenderComponent->SetPSO(EPSOType::Basic);
	}
private:

};
CTestScene::CTestScene()
{
	CActor* Actor = GetWorld()->SpawnActor<CTestActor>(this);
	Actor->GetTransform()->SetPosition(Vector3(0.5f, 0.0f, 0.0f));
}
