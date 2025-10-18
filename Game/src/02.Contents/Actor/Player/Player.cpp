#include "pch.h"
#include "Player.h"
#include "05.Input/InputActionManager.h"
#include "01.Base/World/World.h"

CPlayer::~CPlayer()
{
	
}

void CPlayer::Initalize()
{
	CActor::Initalize();

	Body = GetWorld()->SpawnActor<CCharacter>(this);
	Body->SetAnimator();
	CAnimation* Animation = new CAnimation(true);
	TFrame Frame;
	Frame.ImagePath = L"resources/image/Character/stand/Body0.png";
	Animation->AddFrame(Frame);
	Frame.ImagePath = L"resources/image/Character/stand/Body1.png";
	Animation->AddFrame(Frame);
	Frame.ImagePath = L"resources/image/Character/stand/Body2.png";
	Animation->AddFrame(Frame);
	Frame.ImagePath = L"resources/image/Character/stand/Body1.png";
	Animation->AddFrame(Frame);
	Animation->UnifyFrameDuration(1.0f);

	Body->GetAnimator()->AddAnimation("Basic", std::unique_ptr<CAnimation>(Animation));
	Body->GetAnimator()->SetCurrentAnimation("Basic");

	Head = GetWorld()->SpawnActor<CCharacter>(this);
	Head->GetRenderComponent()->SetImage(L"resources/image/Character/Head_Front.png");
	Head->GetTransform()->SetPosition(Vector3(0.005f, 0.065f, 0.0f));

	Arm = GetWorld()->SpawnActor<CCharacter>(this);
	Arm->GetRenderComponent()->SetImage(L"resources/image/Character/stand/Arm0.png");
	Arm->GetTransform()->SetPosition(Vector3(0.02f, 0.0f, 0.0f));

	//Hand = GetWorld()->SpawnActor<CCharacter>(this);
	//Hand->GetRenderComponent()->SetImage(L"resources/image/Character/stand/Body0.png");

	GetTransform()->SetSpeed(0.002f);

	printf("%d\n", GetType());
}

void CPlayer::Update(float InDeltaTime)
{
	CActor::Update(InDeltaTime);
}

void CPlayer::Destroy()
{
	CActor::Destroy();

	LeftMoveActionValue->Destroy();
	RightMoveActionValue->Destroy();
	UpMoveActionValue->Destroy();
	DownMoveActionValue->Destroy();

	if (Head)
		Head->Destroy();
	if (Body)
		Body->Destroy();
	if (Arm)
		Arm->Destroy();
	if (Hand)
		Hand->Destroy();
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
