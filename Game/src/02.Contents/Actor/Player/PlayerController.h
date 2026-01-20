#pragma once
#include "01.Base/Actor/Actor.h"

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
	}
	void Link(CPlayer& InManagedPlayer);

	void SetupInputComponent(class CInputDispatcher& InInputDispatcher);
	void AddKeyInput(EKeyType InKeyType, const std::function<void()>& InAction);


private:
	CObjectPtr<CPlayer> ManagingPlayer = nullptr;

	class CInputComponent* InputComponent = nullptr;
	class CSkillCaster* SkillCaster = nullptr;

};

