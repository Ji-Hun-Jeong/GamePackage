#include "pch.h"
#include "EditHelper.h"

#include "GameCore.h"
#include "00.App/WindowManager.h"
#include "00.App/MouseManager.h"

void CImageImporter::AddImagePathByWindowManager(CWindowManager& InWindowManager)
{
	std::wstring ImagePath;
	if (CWindowManager::GetInst().TryGetFilePathByDialog(&ImagePath))
	{
		std::string ImagePath_TF = std::string().assign(ImagePath.begin(), ImagePath.end());
		size_t SlashIndex = ImagePath_TF.rfind('\\');
		if (SlashIndex == std::string::npos)
			assert(0);

		std::string ImageName = ImagePath_TF.substr(SlashIndex + 1);

		AddImagePath(ImageName, ImagePath);
	}
}

CStaticActor* CActorGenerator::GenerateStaticActor(const Vector2& InPosition)
{
	CStaticActor* StaticActor = GetWorld()->SpawnActor<CStaticActor>(this);
	StaticActor->GetTransform()->SetPosition(Vector3(InPosition.x, InPosition.y, 1.0f));
	return StaticActor;
}

void CActorTranslator::SetFirstDiff(CMouseManager& InMouseManager, CActor& InTranslatedActor)
{
	const Vector2& Mouse2dPosition = InMouseManager.GetMousePosition();
	Vector3 Mouse3dPosition = Vector3(Mouse2dPosition.x, Mouse2dPosition.y, 1.0f);
	FirstDiff = Mouse3dPosition - InTranslatedActor.GetTransform()->GetPosition();
}

void CActorTranslator::SetFirstDiff(CMouseManager& InMouseManager, const Vector3& InPosition)
{
	const Vector2& Mouse2dPosition = InMouseManager.GetMousePosition();
	Vector3 Mouse3dPosition = Vector3(Mouse2dPosition.x, Mouse2dPosition.y, 1.0f);
	FirstDiff = Mouse3dPosition - InPosition;
}

void CActorTranslator::TranslateActor(CMouseManager& InMouseManager, CActor& InTranslatedActor)
{
	const Vector2& Mouse2dPosition = InMouseManager.GetMousePosition();
	Vector3 Mouse3dPosition = Vector3(Mouse2dPosition.x, Mouse2dPosition.y, 1.0f);
	InTranslatedActor.GetTransform()->SetPosition(Mouse3dPosition - FirstDiff);
}
