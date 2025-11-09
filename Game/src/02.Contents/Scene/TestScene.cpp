#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"

CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(1.0f);
	CActor* Actor = GetWorld()->SpawnActor<CPlayer>(this);

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/Map/MushroomStage/MushroomStage.png");
	BackGround->GetPixelCollider()->SetPixelMap(EPixelMapType::FloorMap, L"resources/image/Map/MushroomStage/Floor.png");
	BackGround->GetPixelCollider()->SetPixelMap(EPixelMapType::LadderMap, L"resources/image/Map/MushroomStage/Ladder.png");
	BackGround->GetPixelCollider()->SetPixelMap(EPixelMapType::WallMap, L"resources/image/Map/MushroomStage/Wall.bmp");
	BackGround->GetPixelCollider()->SetPixelMap(EPixelMapType::MonsterWallMap, L"resources/image/Map/MushroomStage/MonsterWall.bmp");
	BackGround->GetPixelCollider()->SetPixelMap(EPixelMapType::RopeMap, L"resources/image/Map/MushroomStage/Rope.png");
}
