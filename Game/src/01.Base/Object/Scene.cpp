#include "pch.h"
#include "Scene.h"

#include "01.Base/World/World.h"


void CScene::Initalize()
{
	CActor::Initalize();
	MainCamera = SpawnActor<CCamera>(this);
}
