#include "pch.h"
#include "EditUIScene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/BackGround.h"

void CEditUIScene::Initalize()
{
	CScene::Initalize();
	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/UI/Title/Background.png");
	
}
