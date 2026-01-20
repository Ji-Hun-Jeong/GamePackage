#include "pch.h"
#include "PlayerController.h"

#include "02.Contents/Component/Input/InputDispatcher.h"
#include "02.Contents/Skill/SkillCaster.h"

#include "GameCore.h"

CPlayerController::CPlayerController()
{
	InputComponent = AddComponent<CInputComponent>();
	/*SkillCaster = AddComponent<CSkillCaster>();

	CWzLoader WzLoader;
	SkillLoader.LoadMeleeAttackData(WzLoader, "resources/data/Skill/Test.json");*/
}

void CPlayerController::Link(CPlayer& InManagedPlayer)
{
	ManagingPlayer = &InManagedPlayer;

	SkillCaster = ManagingPlayer->GetComponent<CSkillCaster>();
	if (SkillCaster == nullptr)
		SkillCaster = ManagingPlayer->AddComponent<CSkillCaster>();
}

void CPlayerController::SetupInputComponent(CInputDispatcher& InInputDispatcher)
{
	// 여기서는 간단한 이동정도만

	InInputDispatcher.RegistInput(*InputComponent);
}

void CPlayerController::AddKeyInput(EKeyType InKeyType, const std::function<void()>& InAction)
{
	InputComponent->AddAction(InKeyType, EButtonState::Tap, InAction);
}
