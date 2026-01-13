#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "02.Contents/ActorComponent/GroundDetector.h"

#include "01.Base/Actor/Component/RigidBody.h"
#include "01.Base/Actor/Component/Animation/Animator.h"
#include "01.Base/Actor/Component/Animation/AnimatorController.h"

#include "02.Contents/Component/Character.h"

#include "04.Renderer/ImGuiManager.h"

class CPlayer : public CActor
{
	GENERATE_OBJECT(CPlayer)
	DONTCOPY(CPlayer)
public:
	CPlayer();
	~CPlayer();

public:
	void BeginPlay() override;
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);

		/*if (GetKey(EKeyType::A, EButtonState::Hold))
		{
			RigidBody->AddForce(Vector2(-2.0f, 0.0f));
		}
		else if (GetKey(EKeyType::D, EButtonState::Hold))
		{
			RigidBody->AddForce(Vector2(2.0f, 0.0f));
		}
		else if (GetKey(EKeyType::W, EButtonState::Hold))
		{
			RigidBody->AddForce(Vector2(0.0f, 50.0f));
		}
		else if (GetKey(EKeyType::S, EButtonState::Hold))
		{
			RigidBody->AddForce(Vector2(0.0f, -50.0f));
		}*/
	}
	void LateUpdate(float InDeltaTime) override
	{
		CActor::LateUpdate(InDeltaTime);

		GroundDetector->AdjustPlayerPosition(*this);
	}

	void CaptureSnapShot() override
	{
		CActor::CaptureSnapShot();

		// 입력받은 문자열을 저장할 버퍼 (정적 변수나 멤버 변수 권장)
		static char inputBuffer[256] = "";

		ImGui::Begin("Snapshot Event");

		// 엔터키를 쳤을 때만 true를 반환하도록 설정
		if (ImGui::InputText("Command", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			// 여기서 이벤트 발생!
			std::string command = inputBuffer;
			
			Character->PlayAnimation(command);

			// 입력 후 버퍼 비우기 (필요 시)
			memset(inputBuffer, 0, sizeof(inputBuffer));

			// 입력 칸에 계속 포커스를 유지하고 싶다면
			ImGui::SetKeyboardFocusHere(-1);
		}

		ImGui::End();
	}

public:
	bool IsOnGround() const { return GroundDetector->IsOnGround(); }
	void PlayPlayerAnimation(const std::string& InAnimationName) { AnimatorController->PlayIntegrationAnimation(InAnimationName); }

private:
	CStaticActor* Head = nullptr;
	CStaticActor* Body = nullptr;
	CStaticActor* Arm = nullptr;
	CStaticActor* Hand = nullptr;

	CCharacter* Character = nullptr;
private:
	CGroundDetector* GroundDetector = nullptr;
	CAnimatorController* AnimatorController = nullptr;

};

