#pragma once
#include "StaticActor.h"

class CCharacter : public CStaticActor
{
	GENERATE_OBJECT(CCharacter)
		DONTCOPY(CCharacter)
public:
	CCharacter()
		: CStaticActor()
		, Animator(nullptr)
	{
		Animator = AddComponent<CAnimator>();
	}
	virtual ~CCharacter() {}
public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);
		bool bChangeAnimation = Animator->TryChangeCurrentAnimation();

		CAnimation* CurrentAnimation = Animator->GetCurrentAnimation();
		if (bChangeAnimation)
			CurrentAnimation->RequestFrame(0);

		bool bChangeFrame = CurrentAnimation->TryChangeFrame();
		if (bChangeFrame)
		{
			const TFrame& ChangedFrame = CurrentAnimation->GetCurrentFrame();

			if (ChangedFrame.ImagePath.empty() == false)
				RenderComponent->SetDiffuseImage(ChangedFrame.ImagePath);
		}

		CurrentAnimation->UpdateAnimationState(InDeltaTime);
	}
	CAnimator* GetAnimator() const { return Animator; }

protected:
	CAnimator* Animator;
};

