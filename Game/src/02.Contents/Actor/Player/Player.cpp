#include "pch.h"
#include "Player.h"
#include "05.Input/InputActionManager.h"

CPlayer::~CPlayer()
{
	
}

//void CPlayer::Initalize()
//{
//	CActor::Initalize();
//
//	/*Body = SpawnActor<CCharacter>(this);
//	Body->SetAnimator();
//	CAnimation* Animation = new CAnimation(true);
//	TFrame Frame;
//	Frame.ImagePath = L"resources/image/Character/stand/Body0.png";
//	Animation->AddFrame(Frame);
//	Frame.ImagePath = L"resources/image/Character/stand/Body1.png";
//	Animation->AddFrame(Frame);
//	Frame.ImagePath = L"resources/image/Character/stand/Body2.png";
//	Animation->AddFrame(Frame);
//	Frame.ImagePath = L"resources/image/Character/stand/Body1.png";
//	Animation->AddFrame(Frame);
//	Animation->UnifyFrameDuration(1.0f);
//
//	Body->GetAnimator()->AddAnimation("Basic", std::unique_ptr<CAnimation>(Animation));
//	Body->GetAnimator()->SetCurrentAnimation("Basic");
//
//	Head = SpawnActor<CCharacter>(this);
//	Head->GetRenderComponent()->SetDiffuseImage(L"resources/image/Character/Head_Front.png");
//	Head->GetTransform()->SetPosition(Vector3(5.0f, 6.5f, 0.0f));
//
//	Arm = SpawnActor<CCharacter>(this);
//	Arm->GetRenderComponent()->SetDiffuseImage(L"resources/image/Character/stand/Arm0.png");
//	Arm->GetTransform()->SetPosition(Vector3(2.0f, 0.0f, 0.0f));
//
//	GetTransform()->SetSpeed(2.0f);*/
//}

void CPlayer::EndPlay()
{
	CActor::EndPlay();
	//LeftMoveActionValue->Destroy();
	//RightMoveActionValue->Destroy();
	//UpMoveActionValue->Destroy();
	//DownMoveActionValue->Destroy();
}

void CPlayer::Update(float InDeltaTime)
{
	CActor::Update(InDeltaTime);
}

void CPlayer::SetInputAction(CInputActionManager& InInputActionManager)
{
	//LeftMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Left, EButtonState::Hold, [this]()->void
	//	{
	//		GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f));
	//	});
	//RightMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Right, EButtonState::Hold, [this]()->void
	//	{
	//		GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f));
	//	});
	//UpMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Up, EButtonState::Hold, [this]()->void
	//	{
	//		GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f));
	//	});
	//DownMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::Down, EButtonState::Hold, [this]()->void
	//	{
	//		GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f));
	//	});
}
