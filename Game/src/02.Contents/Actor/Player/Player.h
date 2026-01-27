#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "02.Contents/ActorComponent/GroundDetector.h"

#include "01.Base/Actor/Component/RigidBody.h"
#include "02.Contents/Wz/WzPart/WzPart.h"
#include "02.Contents/Wz/WzCharacter/WzAnimation.h"
#include "02.Contents/Wz/WzLoader.h"
#include "02.Contents/Skill/SkillCaster.h"
#include "04.Renderer/ImGuiManager.h"

// 플레이어는 단순히 어떻게 생겨먹었는지 정의하기만 하기
// 다른 정보들은 플레이어가 죽어도 유지되어야하기 때문에 다른걸로 빼는게 나을듯

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

		if (AAnimator)
		{
			AAnimator->PlayCurrentAnimation(InDeltaTime);
			bool bExistCurrentAnimation = AAnimator->IsCurrentAnimExist();
			if (AAnimator->IsFrameChanged() && bExistCurrentAnimation)
			{
				const TWzHumanFrameData& FrameData = AAnimator->GetCurrentFrameData();
				PartsManager->CompositeParts(*FrameData.CharacerFrames, *FrameData.SkinFrames);
			}
		}

	/*	if (GetKey(EKeyType::A, EButtonState::Tap))
			SkillCaster->CastInstantSkill(SkillData);*/

	}
	void LateUpdate(float InDeltaTime) override
	{
		CActor::LateUpdate(InDeltaTime);

		GroundDetector->AdjustPlayerPosition(*this);
	}

	void CaptureSnapShot() override;

public:
	bool IsOnGround() const { return GroundDetector->IsOnGround(); }

private:
	CGroundDetector* GroundDetector = nullptr;
	CWzPartsManager* PartsManager = nullptr;
	
	TSkillData SkillData;
	CSkillCaster* SkillCaster = nullptr;

	CWzLoader CharacterLoader;
	CWzLoader SkinLoader;

	CAnimator<TWzHumanAnimation, TWzHumanFrameData>* AAnimator = nullptr;
};

