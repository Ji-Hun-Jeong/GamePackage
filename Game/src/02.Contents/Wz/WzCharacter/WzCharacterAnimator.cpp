#include "pch.h"
#include "WzCharacterAnimator.h"

#include "GameCore.h"

void CWzCharacterAnimator::PlayCurrentAnimation(float InDeltaTime)
{
	if (CurrentAnimation == nullptr)
		return;
	if (bStop)
		return;

	ProgressTime += InDeltaTime * 0.5f;

	if (CurrentFrameDelay <= ProgressTime * 1000.0f)
		EnterFrame(CurrentFrameIndex + 1);
}

