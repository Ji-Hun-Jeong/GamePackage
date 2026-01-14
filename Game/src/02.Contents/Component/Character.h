#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"
#include "01.Base/Actor/Component/Animation/Animator.h"
#include "Common/Json.h"



class CCharacter : public CComponent
{
	GENERATE_OBJECT(CCharacter)
public:
	CCharacter() = default;
	~CCharacter() = default;

public:
	/*void AddPart(const std::string& InPartName, CPart* InPart = nullptr);
	CPart* GetPart(const std::string& InPartName) const
	{
		auto Iter = Parts.find(InPartName);
		if (Iter == Parts.end())
			return nullptr;
		return Iter->second;
	}
	void RemovePart(const std::string& InPartName)
	{
		if (Parts.contains(InPartName))
			Parts.erase(InPartName);
	}
	void PlayAnimation(const std::string& InAnimName)
	{
		for (auto& PartPair : Parts)
		{
			CPart* Part = PartPair.second;
			CAnimator* Animator = Part->GetAnimator();
			if(Animator->GetAnimation(InAnimName) == nullptr)
				continue;
			Animator->SetCurrentAnimation(InAnimName);
		}
	}
	void InfiniteAnimation(const std::string& InAnimName)
	{
		for (auto& PartPair : Parts)
		{
			CPart* Part = PartPair.second;
			CAnimation* Animation = Part->GetAnimator()->GetAnimation(InAnimName);
			if (Animation == nullptr)
				continue;
			Animation->SetInfinite(true);
		}
	}

private:
	std::unordered_map<std::string, CPart*> Parts;*/

};

