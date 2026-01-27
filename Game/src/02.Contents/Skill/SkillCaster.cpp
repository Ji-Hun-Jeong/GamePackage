#include "pch.h"
#include "SkillCaster.h"
#include "01.Base/Actor/StaticActor.h"

#include "GameCore.h"
#include "02.Contents/Wz/WzLoader.h"
#include "02.Contents/Global/EffectPool.h"

class CInstanceSkillActor : public CStaticActor
{
	GENERATE_OBJECT(CInstanceSkillActor)
public:
	CInstanceSkillActor()
	{
		AddComponent<CAnimator<TAnimation,TFrame>>();
		HitBox = AddComponent<CRectCollider>();
		HitBox->SetDebugRender(true);
	}
	~CInstanceSkillActor() = default;

public:
	void OnCollisionStay(CCollider& InTargetCollider) override
	{
		if (bHit == false)
			return;
		if (HitAction)
			HitAction(InTargetCollider);
	}
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		bHit = false;
		if (Animator->GetCurrentFrameNumber() == 3
			&& Animator->IsFrameChanged())
			bHit = true;

		if (Animator->IsStopped())
			Destroy();
	}
	void SetHitBox(const Vector2 InLeftTop, const Vector2 InRightBottom)
	{
		HitBox->SetRectScale(InRightBottom - InLeftTop);
	}
	void SetEffect(const TSkillEffect& InEffectData)
	{
		const auto& Effects = InEffectData.Anim;
		if (Effects.empty())
			return;

		TAnimation Anim;
		Animator->AddAnimation("Effect", Anim);

		Anim.Frames.reserve(Effects.size());
		for (size_t i = 0; i < Effects.size(); ++i)
		{
			TFrame& Frame = Anim.Frames.emplace_back(TFrame{});
			Frame.Delay = static_cast<float>(Effects[i].Delay) / 1000.0f;
			Frame.ImagePath = Effects[i].OutLink;
			Frame.Offset = CWzUtils::GetWorldPositionFromWzPosition(Frame.ImagePath, Effects[i].Origin);
		}
		Animator->SetCurrentAnimation("Effect");
	}

	void SetHitAction(std::function<void(CCollider&)> InHitAction) { HitAction = InHitAction; }

private:
	CRectCollider* HitBox = nullptr;
	

	bool bHit = false;
	std::function<void(CCollider&)> HitAction;

};

void GenerateHitEffect(const CActor& InTargetActor, const std::vector<TSkillHit>& SkillHitDatas)
{
	CEffector* Effector = CEffectPool::GetInst().GetEffector();
	Effector->GetTransform()->SetPosition(InTargetActor.GetTransform()->GetWorldPosition());
	auto Animator = Effector->AddComponent<CAnimator<TAnimation, TFrame>>();
	TAnimation Anim;//  = Animator->AddAnimationRef("Effect");
	Animator->AddAnimation("Effect", Anim);

	int32_t RandIndex = std::rand() % int32_t(SkillHitDatas.size());
	const TSkillHit& SkillHitData = SkillHitDatas[RandIndex];

	Anim.Frames.reserve(SkillHitData.Anim.size());
	for (size_t i = 0; i < SkillHitData.Anim.size(); ++i)
	{
		TFrame& Frame = Anim.Frames.emplace_back(TFrame{});
		Frame.Delay = static_cast<float>(SkillHitData.Anim[i].Delay) / 1000.0f;
		Frame.ImagePath = SkillHitData.Anim[i].OutLink;
		Frame.Offset = CWzUtils::GetWorldPositionFromWzPosition(Frame.ImagePath, SkillHitData.Anim[i].Origin);
	}
	Animator->SetCurrentAnimation("Effect");
}

void CSkillCaster::CastInstantSkill(const TSkillData& InSkillData)
{
	CInstanceSkillActor* InstanceSkillActor = GetWorld()->SpawnActor<CInstanceSkillActor>(GetOwnerActor());

	InstanceSkillActor->SetHitBox(InSkillData.Common.LeftTop, InSkillData.Common.RightBottom);
	InstanceSkillActor->SetEffect(InSkillData.Effect);
	InstanceSkillActor->SetHitAction([&InSkillData](CCollider& InTargetCollider)->void
		{
			GenerateHitEffect(*InTargetCollider.GetOwnerActor(), InSkillData.Hit);
		});
}
