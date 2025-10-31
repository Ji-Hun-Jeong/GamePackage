#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/Character.h"

class CPlayer : public CActor
{
	GENERATE_OBJECT(CPlayer)
	DONTCOPY(CPlayer)
public:
	CPlayer();
	~CPlayer();

public:
	void BeginPlay() override;
	void SetupInputActionValue(class CInputActionValueCollector& InInputActionValueCollector) override;

private:
	CCharacter* Head;
	CCharacter* Body;
	CCharacter* Arm;
	CCharacter* Hand;

private:
	void InitalizeInputActionValue();

	std::unique_ptr<class CInputActionValue> LeftMoveActionValue;
	std::unique_ptr<class CInputActionValue> RightMoveActionValue;
	std::unique_ptr<class CInputActionValue> UpMoveActionValue;
	std::unique_ptr<class CInputActionValue> DownMoveActionValue;

};

