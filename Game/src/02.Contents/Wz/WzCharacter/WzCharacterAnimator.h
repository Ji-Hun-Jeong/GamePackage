#pragma once
#include "01.Base/Actor/Component/Component.h"

#include "WzAnimation.h"


class CWzCharacterAnimator : public CComponent
{
	GENERATE_OBJECT(CWzCharacterAnimator)
public:
	CWzCharacterAnimator() = default;
	~CWzCharacterAnimator() = default;

public:
	void PlayCurrentAnimation(float InDeltaTime);
	void SetCurrentAnimation(const std::string_view InAnimName, bool bInLoop = false)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName.data());
		if (Iter == WzCharacterAnimations.end())
		{
			std::cout << "[CWzCharacterAnimator::PlayAnimation] 애니메이션을 찾을 수 없습니다: " << InAnimName << std::endl;
			return;
		}
		TWzAnimation& Animation = Iter->second;
		CurrentAnimation = &Animation;

		CurrentAnimation->bLoop = bInLoop;
		EnterFrame(-1);
	}
	TWzAnimation& GetAnimationRef(const std::string& InAnimName)
	{
		if (WzCharacterAnimations.contains(InAnimName) == false)
			WzCharacterAnimations.emplace(InAnimName, InAnimName);
		return WzCharacterAnimations.at(InAnimName);
	}
	TWzAnimation* GetAnimation(const std::string& InAnimName)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
			return nullptr;
		return &Iter->second;
	}
	void AddAnimation(const TWzAnimation& InAnimation)
	{
		auto Iter = WzCharacterAnimations.find(InAnimation.AnimName);
		if (Iter == WzCharacterAnimations.end())
			WzCharacterAnimations.emplace(InAnimation.AnimName, InAnimation);
	}

	bool IsStopped() const { return bStop; }
	// Frame이 변경됐는지는 무조건 PlayAnimation이후에 체크
	bool IsFrameChanged() const { return bFrameChange; }
	bool IsCurrentAnimExist() const { return CurrentAnimation; }
	const TWzFrameData& GetCurrentFrameData() const { return CurrentAnimation->Frames[CurrentFrameIndex]; }
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
		return WzCharacterAnimations.contains(InFindAnimName.data());
	}
private:
	std::unordered_map<std::string, TWzAnimation> WzCharacterAnimations;

	TWzAnimation* CurrentAnimation = nullptr;

	int32_t CurrentFrameIndex = 0;

	float ProgressTime = 0.0f;

	bool bFrameChange = false;
	bool bStop = false;

	float CurrentFrameDelay = 0.0f;

};

