#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"

CTestScene::CTestScene()
{
	GetWorld()->SpawnActor<CActor>(this);
}
