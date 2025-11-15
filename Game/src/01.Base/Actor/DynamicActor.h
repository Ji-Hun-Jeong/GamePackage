#pragma once
#include "StaticActor.h"

class CDynamicActor : public CStaticActor
{
	GENERATE_OBJECT(CDynamicActor)
		DONTCOPY(CDynamicActor)
public:
	CDynamicActor()
		: CStaticActor()
		, Animator(nullptr)
	{
		Animator = AddComponent<CAnimator>();
	}
	virtual ~CDynamicActor() {}
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
				SpriteRenderComponent->SetDiffuseImage(ChangedFrame.ImagePath);
		}

		CurrentAnimation->UpdateAnimationState(InDeltaTime);
	}
	CAnimator* GetAnimator() const { return Animator; }

protected:
	CAnimator* Animator;
};

