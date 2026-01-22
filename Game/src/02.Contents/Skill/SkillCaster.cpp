#include "pch.h"
#include "SkillCaster.h"

#include "GameCore.h"

//class CMeleeAttackActor : public CStaticActor
//{
//	GENERATE_OBJECT(CMeleeAttackActor)
//public:
//	CMeleeAttackActor()
//	{
//		AddComponent<CAnimator>();
//		HitBox = AddComponent<CRectCollider>();
//	}
//	~CMeleeAttackActor() = default;
//
//public:
//	void OnCollisionStay(CCollider& InTargetCollider) override
//	{
//		if (bEffectToOther == false)
//			return;
//		std::cout << "AttackStay\n";
//	}
//	void Update(float InDeltaTime) override
//	{
//		CStaticActor::Update(InDeltaTime);
//
//		bEffectToOther = false;
//		if (Animator->GetCurrentAnimation()->IsFinish())
//		{
//			Destroy();
//			return;
//		}
//
//		CAnimation* CurrentAnimation = Animator->GetCurrentAnimation();
//		if (CurrentAnimation->IsFrameChanged() && CurrentAnimation->GetCurrentFrameNumber() == AttackFrame)
//			bEffectToOther = true;
//		HitBox->SetDebugRender(bEffectToOther);
//	}
//
//	void SetHitBox(const THitBoxData& InHitBox)
//	{
//		HitBox->SetRectScale(InHitBox.RightBottom - InHitBox.LeftTop);
//		AttackFrame = InHitBox.AttackFrameNumber;
//	}
//
//private:
//	size_t AttackFrame = 0;
//	CRectCollider* HitBox = nullptr;
//
//	bool bEffectToOther = false;
//};
//
//void CSkillCaster::MeleeAttack(const TMeleeAttackData& InMeleeAttackData)
//{
//	CMeleeAttackActor* AttackActor = GetWorld()->SpawnActor<CMeleeAttackActor>(GetOwnerActor());
//
//	CAnimator* Animator = AttackActor->AddComponent<CAnimator>();
//	Animator->AddAnimation(InMeleeAttackData.SkillData.Name, InMeleeAttackData.Animation);
//	Animator->SetCurrentAnimation(InMeleeAttackData.SkillData.Name);
//
//	AttackActor->SetHitBox(InMeleeAttackData.HitBoxData);
//}

void DeSerialize(const rapidjson::Value& InData, THitBoxData* OutHitBoxData)
{
	assert(OutHitBoxData);
	if (InData.HasMember("HitBox") && InData["HitBox"].IsObject())
	{
		auto HitBoxData = InData["HitBox"].GetObject();
		auto LeftTop = HitBoxData["LeftTop"].GetArray();
		auto RightBottom = HitBoxData["RightBottom"].GetArray();
		OutHitBoxData->LeftTop = Vector2(LeftTop[0].GetFloat(), LeftTop[1].GetFloat());
		OutHitBoxData->RightBottom = Vector2(RightBottom[0].GetFloat(), RightBottom[1].GetFloat());
		OutHitBoxData->AttackFrameNumber = HitBoxData["AttackFrameNumber"].GetInt();
	}
}
