#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/DynamicActor.h"
#include "02.Contents/ActorComponent/GroundDetector.h"

#include "01.Base/Actor/Component/RigidBody.h"
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

		if (GetKey(EKeyType::A, EButtonState::Hold))
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
			RigidBody->AddForce(Vector2(0.0f, 50.0f));
		}
	}
	void LateUpdate(float InDeltaTime) override
	{
		CActor::LateUpdate(InDeltaTime);

		GroundDetector->AdjustPlayerPosition(*this);
	}

public:
	bool IsOnGround() const { return GroundDetector->IsOnGround(); }

private:
	CStaticActor* Head;
	CDynamicActor* Body;
	CDynamicActor* Arm;
	CDynamicActor* Hand;

	CSpriteRenderComponent* DebugRenderComponent = nullptr;

private:
	CGroundDetector* GroundDetector = nullptr;
	

};

