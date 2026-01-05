#pragma once
#include "01.Base/Actor/Actor.h"
#include "01.Base/Actor/DynamicActor.h"
#include "02.Contents/ActorComponent/GroundDetector.h"

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

		Transform->Move(Vector3(0.0f, -0.1f, 0.0f));

		GroundDetector->AdjustPlayerPosition(*this);

		if (LClicked())
		{
			Transform->Move(Vector3(0.0f, 50.0f, 0.0f));
		}
	}

private:
	CDynamicActor* Head;
	CDynamicActor* Body;
	CDynamicActor* Arm;
	CDynamicActor* Hand;

private:
	CGroundDetector* GroundDetector = nullptr;
};

