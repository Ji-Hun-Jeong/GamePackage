#include "pch.h"
#include "Camera.h"
#include "05.Input/InputActionManager.h"

CCamera::~CCamera()
{

}

//void CCamera::Initalize()
//{
//	CActor::Initalize();
//	//SetRenderComponent();
//}

void CCamera::BeginPlay()
{
	CActor::BeginPlay();
	//GetTransform()->SetSpeed(3.5f);
	//GetTransform()->SetScale(Vector3(float(CameraConst.ScreenWidth), float(CameraConst.ScreenHeight), 1.0f));
	//GetRenderComponent()->SetMesh(-1);
	//GetRenderComponent()->AddVertexConstBuffer(sizeof(CameraConst));
}

void CCamera::EndPlay()
{
	CActor::EndPlay();
	//LeftMoveActionValue->Destroy();
	//RightMoveActionValue->Destroy();
	//UpMoveActionValue->Destroy();
	//DownMoveActionValue->Destroy();
}

void CCamera::FinalUpdate()
{
	CActor::FinalUpdate();
	//Vector2 RectTransform = Vector2(GetTransform()->GetFinalPosition().x, GetTransform()->GetFinalPosition().y);
	//for (auto& TransformEvent : TransformEvents)
	//	TransformEvent(RectTransform);
}

void CCamera::CaptureSnapShot()
{
	//if (GetTransform()->OnVariation() == false)
	//	return;

	//GetTransform()->SetVariation(false);

	//CameraConst.ViewProj = GetRenderComponent()->GetModelMatrix(
	//	GetTransform()->GetFinalPosition(), GetTransform()->GetRotation(), GetTransform()->GetScale()).Invert();

	//GetRenderComponent()->UpdateVertexConstBuffer(1, &CameraConst, sizeof(CameraConst));

	//for (auto& Child : GetChild())
	//	Child->GetTransform()->SetVariation(true);
}

//void CCamera::SetInputAction(CInputActionManager& InInputActionManager)
//{
//	// 만약에 나중에 동적으로 키도 바꾸고 싶으면 그냥 InputActionValue자체에서 키를 바꿀 수 있게(그러면 가지고 있다가 거기다가 다시 세팅가능)
//	LeftMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::A, EButtonState::Hold, [this]()->void
//		{
//			GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f));
//		});
//	RightMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::D, EButtonState::Hold, [this]()->void
//		{
//			GetTransform()->Move(Vector3(1.0f, 0.0f, 0.0f));
//		});
//	UpMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::W, EButtonState::Hold, [this]()->void
//		{
//			GetTransform()->Move(Vector3(0.0f, 1.0f, 0.0f));
//		});
//	DownMoveActionValue = InInputActionManager.AddInputActionValue(EKeyType::S, EButtonState::Hold, [this]()->void
//		{
//			GetTransform()->Move(Vector3(0.0f, -1.0f, 0.0f));
//		});
//}

