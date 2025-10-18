#include "pch.h"
#include "Scene2.h"

#include "01.Base/World/World.h"
#include "02.Contents/Actor/Player/Player.h"

void CScene2::Initalize()
{
	CScene::Initalize();
	GetWorld()->SpawnActor<CPlayer>(this);
}
