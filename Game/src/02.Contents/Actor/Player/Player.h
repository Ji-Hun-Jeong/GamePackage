#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/Character.h"
#include "02.Contents/Actor/Base/BackGround.h"

class CPlayer : public CActor
{
	GENERATE_OBJECT(CPlayer)
	DONTCOPY(CPlayer)
public:
	CPlayer();
	~CPlayer();

public:
	void BeginPlay() override;
	void CaptureSnapShot() override;
	void SetupInputActionValue(class CInputActionValueCollector& InInputActionValueCollector) override;
	void OnCollisionEnter(CCollider& InTargetCollider) override
	{
		if (IsSame<CBackGround>(*InTargetCollider.GetOwner()) == false)
			std::cout << "Hi\n";
	}
	void OnCollisionExit(CCollider& InTargetCollider) override
	{
		std::cout << "Bye\n";
	}

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

	CRectCollider* Collider;
};

