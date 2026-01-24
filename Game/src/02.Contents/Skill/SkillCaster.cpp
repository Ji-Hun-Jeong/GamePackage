#include "pch.h"
#include "SkillCaster.h"
#include "01.Base/Actor/StaticActor.h"

#include "GameCore.h"
#include "02.Contents/Wz/WzLoader.h"

class CSkillHitActor : public CStaticActor
{
	GENERATE_OBJECT(CSkillHitActor)
public:
	CSkillHitActor()
	{
		AddComponent<CAnimator>();
	}
	~CSkillHitActor() = default;

public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		if (Animator->GetCurrentAnimation()->IsFinish())
			Destroy();
	}

private:

};
class CInstanceSkillActor : public CStaticActor
{
	GENERATE_OBJECT(CInstanceSkillActor)
public:
	CInstanceSkillActor()
	{
		AddComponent<CAnimator>();
		HitBox = AddComponent<CRectCollider>();
		HitBox->SetDebugRender(true);
	}
	~CInstanceSkillActor() = default;

public:
	void OnCollisionStay(CCollider& InTargetCollider) override
	{
		if (bHit == false)
			return;
		GenerateHitEffect(*InTargetCollider.GetOwnerActor());
	}
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		bHit = false;
		if (Animator->GetCurrentAnimation()->GetCurrentFrameNumber() == 3
			&& Animator->GetCurrentAnimation()->IsFrameChanged())
			bHit = true;

		if (Animator->GetCurrentAnimation()->IsFinish())
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

		CAnimation& Anim = Animator->AddAnimationRef("Effect");

		Anim.Reserve(Effects.size());
		for (size_t i = 0; i < Effects.size(); ++i)
		{
			TFrame& Frame = Anim.AddFrame();
			Frame.Duration = static_cast<float>(Effects[i].Delay) / 1000.0f;
			Frame.ImagePath = Effects[i].OutLink;
			Frame.Offset = CWzUtils::GetWorldPositionFromOrigin(Frame.ImagePath, Effects[i].Origin);
		}
		Animator->SetCurrentAnimation("Effect");
	}
	void AddHitEffect(const TSkillHit& InHitData)
	{
		SkillHitDatas.push_back(InHitData);
	}

private:
	void GenerateHitEffect(const CActor& InTargetActor)
	{
		CSkillHitActor* HitActor = GetWorld()->SpawnActor<CSkillHitActor>(nullptr);
		HitActor->GetTransform()->SetPosition(InTargetActor.GetTransform()->GetWorldPosition());
		CAnimator* Animator = HitActor->AddComponent<CAnimator>();
		CAnimation& Anim = Animator->AddAnimationRef("Hit");

		int32_t RandIndex = std::rand() % int32_t(SkillHitDatas.size());
		const TSkillHit& SkillHitData = SkillHitDatas[RandIndex];

		Anim.Reserve(SkillHitData.Anim.size());
		for (size_t i = 0; i < SkillHitData.Anim.size(); ++i)
		{
			TFrame& Frame = Anim.AddFrame();
			Frame.Duration = static_cast<float>(SkillHitData.Anim[i].Delay) / 1000.0f;
			Frame.ImagePath = SkillHitData.Anim[i].OutLink;
			Frame.Offset = CWzUtils::GetWorldPositionFromOrigin(Frame.ImagePath, SkillHitData.Anim[i].Origin);
		}
		Animator->SetCurrentAnimation("Hit");
	}

private:
	CRectCollider* HitBox = nullptr;
	std::vector<TSkillHit> SkillHitDatas;

	bool bHit = false;

};

void CSkillCaster::CastInstantSkill(const TSkillData& InSkillData)
{
	CInstanceSkillActor* InstanceSkillActor = GetWorld()->SpawnActor<CInstanceSkillActor>(GetOwnerActor());

	InstanceSkillActor->SetHitBox(InSkillData.Common.LeftTop, InSkillData.Common.RightBottom);
	InstanceSkillActor->SetEffect(InSkillData.Effect);
	for(const auto& HitData : InSkillData.Hit)
		InstanceSkillActor->AddHitEffect(HitData);
}
