#include "pch.h"
#include "TestScene.h"
#include "GameCore.h"
#include "../Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"

#include "02.Contents/Actor/Manager/GroundManager.h"
#include "02.Contents/Actor/Manager/SkillDataLinker.h"

#include "02.Contents/Component/Input/InputDispatcher.h"
#include "02.Contents/Actor/Player/PlayerController.h"
#include "02.Contents/Skill/SkillManager.h"

#include "01.Base/Manager/WzLoader.h"

CTestScene::CTestScene()
{
}

void CTestScene::BeginPlay()
{
	CScene::BeginPlay();

	GetFader()->FadeIn(1.0f);

	/*CActor* Root = GetWorld()->SpawnActor<CActor>(this);
	Root->GetTransform()->SetPosition(Vector3(-160.0f, 180.0f, 0.0f));

	CStaticActor* UI0 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI0->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.backgrnd.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI0, Vector2(1.0f, 0.0f));*/

	/*CStaticActor* UI0 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI0->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.backgrnd.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI0, Vector2(1.0f, 0.0f));
	CStaticActor* UI1 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI1->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.backgrnd2.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI1, Vector2(-5.0f, -22.0f));
	CStaticActor* UI2 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI2->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.backgrnd3.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI2, Vector2(-7.0f, -47.0f));
	CStaticActor* UI3 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI3->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.skillPoint.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI3, Vector2(-212.0f, -29.0f));

	CStaticActor* UI4 = GetWorld()->SpawnActor<CStaticActor>(Root);
	UI4->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/UI/UIWindow2.img/Skill/main/Skill.main.skill0.png");
	CWzUtils::SetWorldPositionFromOrigin(*UI4, Vector2(0.0f, 0.0f));*/

	CPlayer* Player = GetWorld()->SpawnActor<CPlayer>(this);
	Player->GetTransform()->SetPosition(Vector3(0.0f, 50.0f, 0.0f));
	Player->GetComponent<CRigidBody>()->SetGravity(0.0f);

	/*CSkillManager* SkillManager = GetWorld()->SpawnActor<CSkillManager>(this);

	CPlayerController* PlayerController = GetWorld()->SpawnActor<CPlayerController>(this);
	PlayerController->Link(*Player);

	CInputDispatcher* InputDispatcher = GetWorld()->SpawnActor<CInputDispatcher>(this);
	PlayerController->SetupInputComponent(*InputDispatcher);

	CSkillDataLinker* SkillDataLinker = GetWorld()->SpawnActor<CSkillDataLinker>(this);
	SkillDataLinker->SetPlayerController(*PlayerController);
	SkillDataLinker->SetSkillManager(*SkillManager);
	SkillDataLinker->SetupUI(UIManager);*/

	/*CBackGround* BackGround = GetWorld()->SpawnActor<CBackGround>(this);
	BackGround->InitalizeBackGround(L"resources/image/Map/MushroomStage/MushroomStage.png");

	CGroundManager* GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);
	Vector3 FirstPosition = Vector3(-500.0f, -50.0f, 0.0f);

	GroundManager->AddGround(Vector3(0.0f, -50.0f, 0.0f), Vector3(1000.0f, 50.0f, 0.0f));*/
}
