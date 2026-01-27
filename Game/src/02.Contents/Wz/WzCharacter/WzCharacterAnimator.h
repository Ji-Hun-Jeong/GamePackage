#pragma once
#include "01.Base/Actor/Component/Component.h"

#include "WzAnimation.h"
#include "../WzPart/WzSkin.h"

template <typename T_ANIM, typename T_FRAME>
class CCAnimator : public CComponent
{
	GENERATE_OBJECT(CCAnimator)
public:
	CCAnimator() = default;
	~CCAnimator() = default;

	void PlayCurrentAnimation(float InDeltaTime)
	{
		if (CurrentAnimation == nullptr)
			return;
		if (bStop)
			return;

		ProgressTime += InDeltaTime * 0.5f;

		if (CurrentFrameDelay <= ProgressTime * 1000.0f)
			EnterFrame(CurrentFrameIndex + 1);
	}
	void SetCurrentAnimation(const std::string_view InAnimName, bool bInLoop = false)
	{
		auto Iter = Animations.find(InAnimName.data());
		if (Iter == Animations.end())
		{
			std::cout << "[CCAnimator::SetCurrentAnimation] 애니메이션을 찾을 수 없습니다: " << InAnimName << std::endl;
			return;
		}
		CurrentAnimation = &Iter->second;

		CurrentAnimation->bLoop = bInLoop;
		EnterFrame(-1);
	}
	void AddAnimation(const std::string_view InAnimName, const T_ANIM& InAnimation)
	{
		auto Iter = Animations.find(InAnimName.data());
		if (Iter == Animations.end())
			Animations.emplace(InAnimName, InAnimation);
	}

	bool IsStopped() const { return bStop; }
	// Frame이 변경됐는지는 무조건 PlayAnimation이후에 체크
	bool IsFrameChanged() const { return bFrameChange; }
	bool IsCurrentAnimExist() const { return CurrentAnimation; }

	const T_FRAME& GetCurrentFrameData() const { return CurrentAnimation->Frames[CurrentFrameIndex]; }
	void EnterFrame(int32_t InFrameIndex)
	{
		bFrameChange = true;
		int32_t FinalFrame = InFrameIndex;
		if (FinalFrame == CurrentAnimation->Frames.size())
		{
			if (CurrentAnimation->bLoop)
				FinalFrame = 0;
			else
			{
				bStop = true;
				return;
			}
		}

		CurrentFrameIndex = FinalFrame;
		ProgressTime = 0.0f;
		bStop = false;

		if (InFrameIndex < 0)
			CurrentFrameDelay = 0.0f;
		else
			CurrentFrameDelay = CurrentAnimation->Frames[CurrentFrameIndex].Delay;
	}
	bool IsExistAnim(const std::string_view InFindAnimName)
	{
		return Animations.contains(InFindAnimName.data());
	}

private:
	std::unordered_map<std::string, T_ANIM> Animations;

	T_ANIM* CurrentAnimation = nullptr;

	int32_t CurrentFrameIndex = 0;

	float ProgressTime = 0.0f;

	bool bFrameChange = false;
	bool bStop = false;

	float CurrentFrameDelay = 0.0f;

};