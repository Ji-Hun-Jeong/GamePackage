#include "pch.h"
#include "Player.h"
#include "GameCore.h"

CPlayer::CPlayer()
	: Head(nullptr)
	, Body(nullptr)
	, Arm(nullptr)
	, Hand(nullptr)
{
	Transform->SetSpeed(2.0f);
	Transform->SetScale(Vector3(50.0f, 50.0f, 1.0f));
}
CPlayer::~CPlayer()
{
	
}


void CPlayer::BeginPlay()
{
	Body = GetWorld()->SpawnActor<CDynamicActor>(this);
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

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
}
