#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"

#include "02.Contents/Actor/Manager/GroundManager.h"

CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(1.0f);
	CStaticActor* Actor = GetWorld()->SpawnActor<CStaticActor>(this);
	Actor->GetTransform()->SetPosition(Vector3(-100.0f, 50.0f, 0.0f));
	CRectCollider* RectCollider = Actor->AddComponent<CRectCollider>();
	RectCollider->SetRectScale(Vector2(100.0f, 100.0f));
	RectCollider->SetDebugRender(true);

	CAnimator* Animator = Actor->AddComponent<CAnimator>();
	CAnimation& Animation = Animator->AddAnimationRef("walk1");
	TFrame& Frame0 = Animation.AddFrame();
	Frame0.ImagePath = L"resources/image/Player/Alert/0.png";
	TFrame& Frame1 = Animation.AddFrame();
	Frame1.ImagePath = L"resources/image/Player/Alert/1.png";
	TFrame& Frame2 = Animation.AddFrame();
	Frame2.ImagePath = L"resources/image/Player/Alert/2.png";
	TFrame& Frame3 = Animation.AddFrame();
	Frame3.ImagePath = L"resources/image/Player/Alert/3.png";
	Animation.UnifyFrameDuration(0.5f);
	Animation.SetLoop(true);

	Animator->SetCurrentAnimation("walk1");


	CPlayer* Player = GetWorld()->SpawnActor<CPlayer>(this);
	Player->GetTransform()->SetPosition(Vector3(0.0f, 50.0f, 0.0f));
	Player->GetComponent<CRigidBody>()->SetGravity(0.0f);

	/*CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/Map/MushroomStage/MushroomStage.png");

	CGroundManager* GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);
	Vector3 FirstPosition = Vector3(-500.0f, -50.0f, 0.0f);

	GroundManager->AddGround(Vector3(0.0f, -50.0f, 0.0f), Vector3(1000.0f, 50.0f, 0.0f));*/
}
