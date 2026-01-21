#include "pch.h"
#include "Player.h"
#include "GameCore.h"

#include "01.Base/Manager/WzLoader.h"
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
	CWzCharacterLoader& CharacterLoader = CWzCharacterLoader::GetInst();
	CWzLoader WzLoader;
	auto Node = WzLoader.LoadWzData("resources/data/Character/Character.00002000.img.json");
	CharacterLoader.LoadWzCharacterAnimation(*Node, "00002000.img", "walk1", CharacterAnimator);
	/*if (WzLoader.OpenWzData("resources/data/Character/Character.00002000.img.json"))
	{

		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "walk2", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "stand1", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "stand2", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "swingO1", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "swingO2", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "swingO3", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "alert", CharacterAnimator);
		CharacterLoader.LoadWzCharacterAnimation(WzLoader.GetLoadData(), "Base", "fly", CharacterAnimator);

		WzLoader.CloseWzData();
		CharacterAnimator->SetCurrentAnimation("alert", true);
	}*/

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
}
