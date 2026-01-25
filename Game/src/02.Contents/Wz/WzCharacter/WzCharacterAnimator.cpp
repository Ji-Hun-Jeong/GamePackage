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

namespace Wz
{
	bool ParseWzCharacterAnimation(const JValue& InValue, TWzCharacterAnimation* OutCharacterAnimation)
	{
		if (InValue.IsObject() == false)
			return false;
		const auto& AnimObject = InValue.GetObject();
		OutCharacterAnimation->Frames.resize(AnimObject.MemberCount());
		size_t i = 0;
		for (const auto& Frame : AnimObject)
		{
			const std::string_view FrameNumber = Frame.name.GetString();
			if (ParseCharacterFrame(Frame.value, &OutCharacterAnimation->Frames[i++]) == false)
				return false;
		}
		return true;
	}
}