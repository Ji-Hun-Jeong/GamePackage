#include "pch.h"
#include "Scene.h"
#include "GameCore.h"

CScene::CScene()
{
	MainCamera = GetWorld()->SpawnActor<CCamera>(this);
	Fader = GetWorld()->SpawnActor<CFader>(this);
	Fader->GetRenderComponent()->SetLayer(1);
}

void CScene::LoadNewSceneWithFadeOut(CClass* InSceneClass, float InHoldingTime)
{
	Fader->SetStateEndEvent([this, InSceneClass]()->void
		{
			GetWorld()->LoadSceneByClass(InSceneClass);
		});
	Fader->FadeOut(InHoldingTime);
}
