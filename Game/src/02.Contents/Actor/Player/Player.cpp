#include "pch.h"
#include "Player.h"
#include "GameCore.h"

CPlayer::CPlayer()
{
	Transform->SetScale(Vector3(37.0f, 37.0f, 1.0f));

	AddComponent<CSpriteRenderComponent>();
	SetLineActor();
	SpriteRenderComponent->SetColor(Vector3(0.0f, 1.0f, 0.0f), 1.0f);

	AddComponent<CRigidBody>();

	CharacterAnimator = AddComponent<CWzCharacterAnimator>();
}

CPlayer::~CPlayer()
{
	
}


void CPlayer::BeginPlay()
{
	CWzCharacterLoader& Loader = CWzCharacterLoader::GetInst();
	Loader.OpenWzData("resources/data/Character/Character.00002000.img.json");
	Loader.LoadWzAnimation("Base", "walk1", CharacterAnimator);
	Loader.CloseWzData();

	CharacterAnimator->SetCurrentAnimation("walk1", true);

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
}
