#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"

#include "02.Contents/Actor/Manager/GroundManager.h"
CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(1.0f);
	CPlayer* Player = GetWorld()->SpawnActor<CPlayer>(this);
	Player->GetTransform()->SetPosition(Vector3(0.0f, 50.0f, 0.0f));

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/Map/MushroomStage/MushroomStage.png");

	CGroundManager* GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);
	Vector3 FirstPosition = Vector3(-500.0f, -50.0f, 0.0f);

	GroundManager->AddGround(Vector3(0.0f, -50.0f, 0.0f), Vector3(1000.0f, 50.0f, 0.0f));
}
