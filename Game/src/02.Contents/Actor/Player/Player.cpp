#include "pch.h"
#include "Player.h"
#include "05.Input/InputActionManager.h"

CPlayer::~CPlayer()
{

}

void CPlayer::Initalize()
{
	CCharacter::Initalize();

	GetRenderComponent()->SetImage(L"resources/image/Player/Attack/0.png");
	GetTransform()->SetSpeed(0.002f);
}

void CPlayer::Update(float InDeltaTime)
{
	CCharacter::Update(InDeltaTime);
}

void CPlayer::Destroy()
{
	CCharacter::Destroy();
	LeftMoveActionValue->Destroy();
	RightMoveActionValue->Destroy();
	UpMoveActionValue->Destroy();
	DownMoveActionValue->Destroy();
}

void CPlayer::SetInputAction(CInputActionManager& InInputActionManager)
{
	LeftMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Left, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f));
		});
	RightMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Right, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f));
		});
	UpMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Up, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f));
		});
	DownMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Down, EButtonState::Hold, [this]()->void
		{
			GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f));
		});
}
