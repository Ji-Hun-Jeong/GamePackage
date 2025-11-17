#include "pch.h"
#include "ActorGenerator.h"

#include "GameCore.h"
#include "00.App/WindowManager.h"

CStaticActor* CActorGenerator::GenerateStaticActor(const std::wstring& InImagePath, const Vector2& InPosition)
{
	if (InImagePath.empty())
		return nullptr;

	CStaticActor* StaticActor = GetWorld()->SpawnActor<CStaticActor>(this);
	StaticActor->GetSpriteRenderComponent()->SetDiffuseImage(InImagePath);
	StaticActor->GetTransform()->SetPosition(Vector3(InPosition.x, InPosition.y, 1.0f));

	GeneratedActors.push_back(StaticActor);

	return StaticActor;
}

CStaticActor* CActorGenerator::GenerateStaticActor(const Vector2& InPosition)
{
	if (GeneratedActorImagePath.empty())
		return nullptr;

	CStaticActor* StaticActor = GetWorld()->SpawnActor<CStaticActor>(this);
	StaticActor->GetSpriteRenderComponent()->SetDiffuseImage(GeneratedActorImagePath);
	StaticActor->GetTransform()->SetPosition(Vector3(InPosition.x, InPosition.y, 1.0f));

	GeneratedActors.push_back(StaticActor);

	return StaticActor;
}

void CActorGenerator::EraseActor(CStaticActor& InErasedActor)
{
	if (GeneratedActors.empty())
		return;

	for (auto Iter = GeneratedActors.begin(); Iter != GeneratedActors.end(); ++Iter)
	{
		CActor* Actor = *Iter;
		if (Actor == &InErasedActor)
		{
			Actor->Destroy();
			GeneratedActors.erase(Iter);
			return;
		}
	}
}

void CActorGenerator::SetGeneratedActorImagePathByWindowManager(CWindowManager& InWindowManager)
{
	std::wstring ImagePath;
	if (CWindowManager::GetInst().TryGetFilePathByDialog(&ImagePath))
	{
		std::string ImagePath_TF = std::string().assign(ImagePath.begin(), ImagePath.end());
		size_t SlashIndex = ImagePath_TF.rfind('\\');
		if (SlashIndex == std::string::npos)
			assert(0);

		std::string ImageName = ImagePath_TF.substr(SlashIndex + 1);
		LoadedImagePaths.emplace(ImageName, ImagePath);

		GeneratedActorImagePath = ImagePath;
	}
}
