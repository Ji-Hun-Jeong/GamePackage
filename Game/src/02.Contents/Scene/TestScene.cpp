#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"

void CTestScene::Initalize()
{
	CScene::Initalize();
	GetWorld()->SpawnActor<CActor>(this);
}
