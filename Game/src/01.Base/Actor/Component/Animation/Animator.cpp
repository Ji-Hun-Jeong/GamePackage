#include "pch.h"
#include "Animator.h"

#include "01.Base/Actor/Actor.h"

void CAnimator::PlayAnimation(float InDeltaTime)
{
	if (CurrentAnimation == nullptr)
		return;
	if (CurrentAnimation->IsFinish())
		return;

	bool bChangeFrame = CurrentAnimation->IsFrameChanged();
	if (bChangeFrame)
	{
		const TFrame& ChangedFrame = CurrentAnimation->GetCurrentFrame();

		CActor* OwnerActor = GetOwnerActor();
		OwnerActor->GetTransform()->SetPosition(ChangedFrame.Offset);
		if (OwnerActor->GetSpriteRenderComponent())
			OwnerActor->GetSpriteRenderComponent()->SetDiffuseImage(ChangedFrame.ImagePath);
		if (ChangedFrame.FrameAction)
			ChangedFrame.FrameAction();
	}

	CurrentAnimation->UpdateAnimationState(InDeltaTime);
}
