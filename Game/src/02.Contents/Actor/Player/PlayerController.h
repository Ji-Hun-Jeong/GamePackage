#pragma once
#include "01.Base/Actor/Actor.h"

#include "02.Contents/Skill/SkillCaster.h"
#include "02.Contents/Skill/SkillLoader.h"

#include "Player.h"

class CPlayerController : public CActor
{
	GENERATE_OBJECT(CPlayerController)
public:
	CPlayerController();
	~CPlayerController() = default;

public:
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);

		if (ManagingPlayer == nullptr)
			return;

		Transform->SetPosition(ManagingPlayer->GetTransform()->GetPosition());
	}
	void Link(CPlayer& InManagedPlayer)
	{
		ManagingPlayer = &InManagedPlayer;
	}

	void SetupInputComponent(class CInputDispatcher& InInputDispatcher);

private:
	CPlayer* ManagingPlayer = nullptr;

	class CInputComponent* InputComponent = nullptr;

	CSkillCaster* SkillCaster = nullptr;
	CSkillLoader SkillLoader;
};

