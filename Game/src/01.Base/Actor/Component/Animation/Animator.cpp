#include "pch.h"
#include "Animator.h"

#include "01.Base/Actor/Actor.h"

CAnimator::CAnimator()
	: CurrentAnimation(nullptr)
	, CurrentAnimationChangeRequest(nullptr)
{}

void CAnimator::PlayAnimation(float InDeltaTime)
{
	bool bChangeAnimation = TryChangeCurrentAnimation();

	CAnimation* CurrentAnimation = GetCurrentAnimation();
	if (bChangeAnimation)
		CurrentAnimation->RequestFrame(0);

	bool bChangeFrame = CurrentAnimation->TryChangeFrame();
	if (bChangeFrame)
	{
		const TFrame& ChangedFrame = CurrentAnimation->GetCurrentFrame();

		CActor* OwnerActor = GetOwnerActor();
		OwnerActor->GetTransform()->SetPosition(ChangedFrame.Offset);
		if (ChangedFrame.ImagePath.empty() == false && OwnerActor->GetSpriteRenderComponent())
			OwnerActor->GetSpriteRenderComponent()->SetDiffuseImage(ChangedFrame.ImagePath);
		if (ChangedFrame.FrameAction)
			ChangedFrame.FrameAction();
	}

	CurrentAnimation->UpdateAnimationState(InDeltaTime);
}
