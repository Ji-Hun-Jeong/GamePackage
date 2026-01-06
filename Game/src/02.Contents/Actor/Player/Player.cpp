#include "pch.h"
#include "Player.h"
#include "GameCore.h"

CPlayer::CPlayer()
	: Head(nullptr)
	, Body(nullptr)
	, Arm(nullptr)
	, Hand(nullptr)
{
	Transform->SetScale(Vector3(37.0f, 37.0f, 1.0f));

	AddComponent<CSpriteRenderComponent>();
	SetLineActor();
	SpriteRenderComponent->SetColor(Vector3(0.0f, 1.0f, 0.0f), 1.0f);

	AddComponent<CRigidBody>();
}
CPlayer::~CPlayer()
{
	
}


void CPlayer::BeginPlay()
{
	{
		Body = GetWorld()->SpawnActor<CStaticActor>(this);
		Body->AddComponent<CAnimator>();

		auto Animation = std::make_unique<CAnimation>(true);
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

		Body->GetAnimator()->AddAnimation("Basic", std::move(Animation));
		Body->GetAnimator()->SetCurrentAnimation("Basic");
	}
	{
		Head = GetWorld()->SpawnActor<CStaticActor>(this);
		Head->GetSpriteRenderComponent()->SetDiffuseImage(L"resources/image/Character/Head_Front.png");
		Head->GetTransform()->SetPosition(Vector3(3.0f, 32.0f, 0.0f));
	}

	GroundDetector = GetWorld()->SpawnActor<CGroundDetector>(this);
	GroundDetector->SetDetectScale(Vector2(70.0f, 10.0f));
	GroundDetector->GetTransform()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
}
