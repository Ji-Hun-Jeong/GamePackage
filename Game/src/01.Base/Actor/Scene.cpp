#include "pch.h"
#include "Scene.h"
#include "GameCore.h"

CScene::CScene()
{
	MainCamera = GetWorld()->SpawnActor<CCamera>(this);
	Fader = GetWorld()->SpawnActor<CFader>(this);
	Fader->GetTransform()->SetScale(Vector3(1280.0f, 960.0f, 0.0f));
	Fader->GetRenderComponent()->SetLayer(1);
}
