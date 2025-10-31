#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"

CTestScene::CTestScene()
{
	CActor* Actor = GetWorld()->SpawnActor<CPlayer>(this);

}
