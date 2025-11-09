#include "pch.h"
#include "Player.h"
#include "GameCore.h"
#include "05.Input/InputActionManager.h"

CPlayer::CPlayer()
	: Head(nullptr)
	, Body(nullptr)
	, Arm(nullptr)
	, Hand(nullptr)
	, LeftMoveActionValue(nullptr)
	, RightMoveActionValue(nullptr)
	, UpMoveActionValue(nullptr)
	, DownMoveActionValue(nullptr)
{
	InitalizeInputActionValue();

	/*Head = GetWorld()->SpawnActor<CCharacter>(this);
	Head->GetRenderComponent()->SetDiffuseImage(L"resources/image/Character/Head_Front.png");
	Head->GetTransform()->SetPosition(Vector3(5.0f, 6.5f, 0.0f));

	Arm = GetWorld()->SpawnActor<CCharacter>(this);
	Arm->GetRenderComponent()->SetDiffuseImage(L"resources/image/Character/stand/Arm0.png");
	Arm->GetTransform()->SetPosition(Vector3(2.0f, 0.0f, 0.0f));*/

	Transform->SetSpeed(2.0f);

	PixelCollider = AddComponent<CPixelCollider>();
	PixelCollider->StartUse();
}
CPlayer::~CPlayer()
{
	
}


void CPlayer::BeginPlay()
{
	Body = GetWorld()->SpawnActor<CCharacter>(this);
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
}

void CPlayer::FinalUpdate()
{
	CActor::FinalUpdate();
	PixelCollider->SetPosition(Transform->GetFinalPosition());
}

void CPlayer::SetupInputActionValue(CInputActionValueCollector& InInputActionValueCollector)
{
	InInputActionValueCollector.PushInputActionValue(*LeftMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*RightMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*UpMoveActionValue);
	InInputActionValueCollector.PushInputActionValue(*DownMoveActionValue);
}

void CPlayer::InitalizeInputActionValue()
{
	LeftMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f)); });
	LeftMoveActionValue->AddKeyCondition({EKeyType::A, EButtonState::Hold});

	RightMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f)); });
	RightMoveActionValue->AddKeyCondition({ EKeyType::D, EButtonState::Hold });

	UpMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f)); });
	UpMoveActionValue->AddKeyCondition({ EKeyType::W, EButtonState::Hold });

	DownMoveActionValue = std::make_unique<CInputActionValue>([this]()->void {GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f)); });
	DownMoveActionValue->AddKeyCondition({ EKeyType::S, EButtonState::Hold });
}
