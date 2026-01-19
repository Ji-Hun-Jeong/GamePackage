#include "pch.h"
#include "PlayerController.h"

#include "02.Contents/Component/Input/InputDispatcher.h"

CPlayerController::CPlayerController()
{
	InputComponent = AddComponent<CInputComponent>();
	SkillCaster = AddComponent<CSkillCaster>();

	CWzLoader WzLoader;
	SkillLoader.LoadMeleeAttackData(WzLoader, "resources/data/Skill/Test.json");
}

void CPlayerController::SetupInputComponent(CInputDispatcher& InInputDispatcher)
{
	InputComponent->AddAction(EKeyType::Ctrl, EButtonState::Tap, [this]()->void
		{
			auto Data = SkillLoader.GetMeleeAttackData("stabO1");
			SkillCaster->MeleeAttack(*Data);
		});

	InInputDispatcher.RegistInput(*InputComponent);
}
