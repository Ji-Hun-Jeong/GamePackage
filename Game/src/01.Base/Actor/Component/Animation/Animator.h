#pragma once
#include "../Component.h"
#include "Animation.h"

class CAnimator : public CComponent
{
	DONTCOPY(CAnimator)
	GENERATE_OBJECT(CAnimator)
public:
	CAnimator()
		: CurrentAnimation(nullptr)
		, CurrentAnimationChangeRequest(nullptr)
	{}
	~CAnimator() = default;

public:
	void AddAnimation(const std::string& InAnimationName, std::unique_ptr<CAnimation> InAnimation)
	{
		assert(InAnimation);
		Animations.insert({ InAnimationName, std::move(InAnimation) });
	}
	void SetCurrentAnimation(const std::string& InAnimation)
	{
		auto Iter = Animations.find(InAnimation);
		if (Iter == Animations.end())
			assert(0);

		CurrentAnimationChangeRequest = Iter->second.get();
	}
	void SetCurrentAnimation(CAnimation& InAnimation)
	{
		CurrentAnimationChangeRequest = &InAnimation;
	}

	bool TryChangeCurrentAnimation()
	{
		if (CurrentAnimationChangeRequest == nullptr)
			return false;

		CurrentAnimation = CurrentAnimationChangeRequest;
		CurrentAnimationChangeRequest = nullptr;

		return true;
	}
	CAnimation* GetCurrentAnimation() const { return CurrentAnimation; }
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		CSerializer AnimatorData;

		CSerializer AnimationArray = CSerializer::array();
		for (auto& Animation : Animations)
		{
			CSerializer AnimationData;
			Animation.second->SaveFrameData(AnimationData);
			AnimationArray.push_back(AnimationData);
		}
		InSerializer = AnimationArray;
	}

private:
	std::map<std::string, std::unique_ptr<CAnimation>> Animations;
	CAnimation* CurrentAnimation;

	CAnimation* CurrentAnimationChangeRequest;


};

