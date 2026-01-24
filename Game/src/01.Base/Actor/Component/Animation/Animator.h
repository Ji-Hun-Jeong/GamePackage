#pragma once
#include "../Component.h"
#include "Animation.h"

class CAnimator : public CComponent
{
	DONTCOPY(CAnimator)
	GENERATE_OBJECT(CAnimator)
public:
	CAnimator() = default;
	~CAnimator() = default;

public:
	void PlayAnimation(float InDeltaTime);
	CAnimation& AddAnimationRef(const std::string& InAnimationName)
	{
		if (Animations.contains(InAnimationName) == false)
			Animations.emplace(InAnimationName, CAnimation{});
		auto Iter = Animations.find(InAnimationName);
		CurrentAnimation = &Iter->second;
		return Iter->second;
	}
	void AddAnimation(const std::string& InAnimationName, const CAnimation& InAnimation)
	{
		Animations.emplace(InAnimationName, InAnimation);
	}
	void SetCurrentAnimation(const std::string& InAnimation)
	{
		auto Iter = Animations.find(InAnimation);
		if (Iter == Animations.end())
			assert(0);

		CurrentAnimation = &Iter->second;
		CurrentAnimation->EnterFrame(0);
	}

	CAnimation* GetCurrentAnimation() const { return CurrentAnimation; }
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		//CSerializer AnimatorData;

		//CSerializer AnimationArray = CSerializer::array();
		//for (auto& Animation : Animations)
		//{
		//	CSerializer AnimationData;
		//	//Animation.second->Serialize(AnimationData);
		//	AnimationArray.push_back(AnimationData);
		//}
		//InSerializer = AnimationArray;
	}
	CAnimation* GetAnimation(const std::string& InAnimationName) 
	{
		auto Iter = Animations.find(InAnimationName);
		if (Iter == Animations.end())
			return nullptr;
		return &Iter->second;
	}
	void Reset()
	{
		Animations.clear();
		CurrentAnimation = nullptr;
	}

private:
	std::map<std::string, CAnimation> Animations;
	CAnimation* CurrentAnimation = nullptr;

};

