#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"
#include <codecvt> // for wstring_convert
#include <locale>  // for codecvt_utf8

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(0.0f);

	TileManager = GetWorld()->SpawnActor<CTileManager>();
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	TileManager->FindFocusTile();
	CTile* FocusTile = TileManager->GetFocusTile();
	if (FocusTile && LHold() && CurrentActorImagePath.empty() == false)
		FocusTile->PutOnActor(CurrentActorImagePath, TilePutMode);
	if (FocusTile && RHold())
		FocusTile->RevertPutOn();

	if (bLayTiles)
	{
		TileManager->LayTiles(TileWidth, TileHeight, TileRow, TileCol);
		bLayTiles = false;
	}

	if (bOpenWindowDialog)
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
		}
		bOpenWindowDialog = false;
	}

}
