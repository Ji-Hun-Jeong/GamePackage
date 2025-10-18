#include "pch.h"
#include "Scene.h"

#include "01.Base/World/World.h"
#include "01.Base/Object/Camera.h"

void CScene::Initalize()
{
	CActor::Initalize();
	GetWorld()->SpawnActor<CCamera>(this);
}
