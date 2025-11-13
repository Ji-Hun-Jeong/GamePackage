#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"

CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(1.0f);
	CActor* Actor = GetWorld()->SpawnActor<CPlayer>(this);

	CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/Map/MushroomStage/MushroomStage.png");
	BackGround->GetTransform()->SetScale(Vector3(0.5f, 0.5f, 1.0f));
	CRectCollider* RectCollider = BackGround->AddComponent<CRectCollider>();
	RectCollider->SetRectScale(Vector2(690.0f, 480.0f));
}
