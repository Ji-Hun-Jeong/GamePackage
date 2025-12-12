#include "pch.h"
#include "LadderEditState.h"

#include "GameCore.h"
CLadderEditState::CLadderEditState()
{
	LadderEditor = GetWorld()->SpawnActor<CLadderEditor>(this);

	LadderEditor->SetHeadImagePath(L"resources/image/Tile/ladder/ladder0.png");
	LadderEditor->AddBodyImagePath(L"resources/image/Tile/ladder/ladder1.png");
	LadderEditor->AddBodyImagePath(L"resources/image/Tile/ladder/ladder2.png");
	LadderEditor->SetFootImagePath(L"resources/image/Tile/ladder/ladder3.png");

	LadderMarker = GetWorld()->SpawnActor<CStaticActor>(this);
	LadderMarker->SetLineActor();
}