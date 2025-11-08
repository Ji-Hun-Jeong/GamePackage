#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"

CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(5.0f);
	CActor* Actor = GetWorld()->SpawnActor<CPlayer>(this);
}
