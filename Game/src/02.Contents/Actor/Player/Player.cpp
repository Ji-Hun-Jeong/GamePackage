#include "pch.h"
#include "Player.h"
#include "GameCore.h"

#include "02.Contents/Skill/Skill.h"
#include "02.Contents/Wz/WzLoader.h"

CPlayer::CPlayer()
{
	Transform->SetScale(Vector3(37.0f, 37.0f, 1.0f));

	AddComponent<CSpriteRenderComponent>();
	SetLineActor();
	SpriteRenderComponent->SetColor(Vector3(0.0f, 1.0f, 0.0f), 1.0f);
	AddComponent<CRigidBody>();

	CharacterAnimator = AddComponent<CWzCharacterAnimator>();
	PartsManager = AddComponent<CWzPartsManager>();
	SkillCaster = AddComponent<CSkillCaster>();
}

CPlayer::~CPlayer()
{

}

void CPlayer::BeginPlay()
{
	CWzLoader WzLoader;
	/*WzLoader.OpenWzData("resources/data/Skill/Skill.2410.img.json");
	JValue* SkillValue;
	if (WzLoader.GetLoadData("skill", &SkillValue))
	{
		SkillLoad::DeSerializeSkillData(*SkillValue, "24101000", &SkillData);
	}*/
	WzLoader.OpenWzData("resources/data/Character/Character.00002000.img.json");
	JValue* AnimValue;
	if (WzLoader.GetLoadData("walk1", &AnimValue))
	{
		TWzCharacterAnimation Anim;
		Anim.AnimName = "walk1";
		Wz::ParseWzCharacterAnimation(*AnimValue, &Anim);
		CharacterAnimator->AddAnimation(Anim);
	}
	CharacterAnimator->SetCurrentAnimation("walk1", true);
	/*CharacterAnimator->AddAnimation(WzLoader, "walk1");
	CharacterAnimator->AddAnimation(WzLoader, "walk2");
	CharacterAnimator->AddAnimation(WzLoader, "stand1");
	CharacterAnimator->AddAnimation(WzLoader, "stand2");
	CharacterAnimator->AddAnimation(WzLoader, "swingO1");
	CharacterAnimator->AddAnimation(WzLoader, "swingO2");
	CharacterAnimator->AddAnimation(WzLoader, "swingO3");
	CharacterAnimator->SetCurrentAnimation("walk1", true);*/

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));

}
