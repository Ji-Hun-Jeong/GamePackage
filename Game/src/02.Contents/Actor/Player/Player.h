#pragma once
#include "01.Base/Object/Actor.h"
#include "01.Base/Object/Character.h"

class CPlayer : public CActor
{
	GENERATE_OBJECT()
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
	void Initalize() override;
	void Update(float InDeltaTime) override;
	void Destroy() override;
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

