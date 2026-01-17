#include "pch.h"
#include "Skill.h"

#include "GameCore.h"

class CAttackActor : public CStaticActor
{
	GENERATE_OBJECT(CAttackActor)
public:
	CAttackActor()
	{
		AddComponent<CAnimator>();
	}
	~CAttackActor() = default;

public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		if (Animator->GetCurrentAnimation()->IsFinish())
			Destroy();
	}

private:

};

void Attack(CActor* InOwner)
{
	CAttackActor* AttackActor = InOwner->GetWorld()->SpawnActor<CAttackActor>(InOwner);
	AttackActor->GetTransform()->SetPosition(Vector3(100.0f, 0.0f, 0.0f));
	CAnimator* Animator = AttackActor->GetAnimator();
	CAnimation& Animation = Animator->GetAnimationRef("stabO1");

	float Um = 100.0f;
	TFrame& Frame0 = Animation.AddFrame();
	Frame0.Duration = 30.0f / Um;
	Frame0.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/0_30.png";

	TFrame& Frame1 = Animation.AddFrame();
	Frame1.Duration = 30.0f / Um;
	Frame1.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/30_60.png";

	TFrame& Frame2 = Animation.AddFrame();
	Frame2.Duration = 30.0f / Um;
	Frame2.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/60_90.png";

	TFrame& Frame3 = Animation.AddFrame();
	Frame3.Duration = 30.0f / Um;
	Frame3.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/90_120.png";

	TFrame& Frame4 = Animation.AddFrame();
	Frame4.Duration = 30.0f / Um;
	Frame4.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/120_150.png";

	TFrame& Frame5 = Animation.AddFrame();
	Frame5.Duration = 30.0f / Um;
	Frame5.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/150_180.png";

	TFrame& Frame6 = Animation.AddFrame();
	Frame6.Duration = 50.0f / Um;
	Frame6.ImagePath = L"resources/image/Character/AfterImage/axe.img/stabO1/180_230.png";

	Animator->SetCurrentAnimation("stabO1");
}