#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/Character.h"

class CPlayer : public CActor
{
	GENERATE_OBJECT(CPlayer)
	DONTCOPY(CPlayer)
public:
	CPlayer()
		: Head(nullptr)
		, Body(nullptr)
		, Arm(nullptr)
		, Hand(nullptr)
		, LeftMoveActionValue(nullptr)
		, RightMoveActionValue(nullptr)
		, UpMoveActionValue(nullptr)
		, DownMoveActionValue(nullptr)
	{}
	~CPlayer();

public:
	void EndPlay() override;
	void Update(float InDeltaTime) override;
	void SetInputAction(class CInputActionManager& InInputActionManager);

private:
	CCharacter* Head;
	CCharacter* Body;
	CCharacter* Arm;
	CCharacter* Hand;

private:
	class CInputActionValue* LeftMoveActionValue;
	class CInputActionValue* RightMoveActionValue;
	class CInputActionValue* UpMoveActionValue;
	class CInputActionValue* DownMoveActionValue;

};

