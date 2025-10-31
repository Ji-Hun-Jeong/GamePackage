#include "pch.h"
#include "Scene.h"
#include "GameCore.h"

//void CScene::Initalize()
//{
//	CActor::Initalize();
//	//MainCamera = SpawnActor<CCamera>(this);
//}

CScene::CScene()
{
	MainCamera = GetWorld()->SpawnActor<CCamera>(this);
}
