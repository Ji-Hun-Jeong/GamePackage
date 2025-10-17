#include "pch.h"
#include "Player.h"

void CPlayer::Initalize()
{
	CCharacter::Initalize();

	GetRenderComponent()->SetImage(L"resources/image/Player/Attack/0.png");
	GetTransform()->SetSpeed(0.0002f);
	GetTransform()->SetScale(Vector3(1.5f));
}

void CPlayer::Update(float InDeltaTime)
{
	CCharacter::Update(InDeltaTime);

	GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f));
}
