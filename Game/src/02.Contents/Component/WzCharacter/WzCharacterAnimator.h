#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"
#include "WzCharacterAnimation.h"
#include "WzPartsManager.h"

class CWzCharacterAnimator : public CComponent
{
	GENERATE_OBJECT(CWzCharacterAnimator)
public:
	CWzCharacterAnimator() = default;
	~CWzCharacterAnimator() = default;

public:
	void InitalizeComponent() override;
	void Update(float InDeltaTime);
	void SetCurrentAnimation(const std::string& InAnimName, bool bInLoop = false)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
		{
			std::cout << "[CWzCharacterAnimator::PlayAnimation] 애니메이션을 찾을 수 없습니다: " << InAnimName << std::endl;
			return;
		}
		CWzCharacterAnimation& Animation = Iter->second;
		CurrentAnimation = &Animation;

		CurrentAnimation->SetLoop(bInLoop);
		CurrentAnimation->EnterFrame(0);
	}
	CWzCharacterAnimation& GetAnimationRef(const std::string& InAnimName)
	{
		if (WzCharacterAnimations.contains(InAnimName) == false)
			WzCharacterAnimations.emplace(InAnimName, InAnimName);
		return WzCharacterAnimations.at(InAnimName);
	}
	CWzCharacterAnimation* GetAnimation(const std::string& InAnimName)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
			return nullptr;
		return &Iter->second;
	}
	void AddAnimation(const CWzCharacterAnimation& InAnimation)
	{
		auto Iter = WzCharacterAnimations.find(InAnimation.GetAnimName());
		if (Iter == WzCharacterAnimations.end())
			WzCharacterAnimations.emplace(InAnimation.GetAnimName(), InAnimation);
	}
	void AddAnimation(CWzLoader& InWzLoader, const std::string_view InFindAnimName)
	{
		CWzCharacterAnimation Animation;
		if (DeserializeAnimation(InWzLoader, InFindAnimName, &Animation))
			AddAnimation(Animation);
	}

	static bool DeserializeAnimation(CWzLoader& InWzLoader, const std::string_view InFindAnimName
		, CWzCharacterAnimation* OutAnimation)
	{
		assert(OutAnimation);
		if (InWzLoader.IsOpen() == false)
			return false;

		const auto& Document = InWzLoader.GetLoadData();

		if (Document.HasMember("dir") == false)
			return false;

		const auto& ImgValue = Document["dir"];
		if (ImgValue.IsObject() == false)
			return false;

		const auto& ImgObject = ImgValue.GetObject();
		if (ImgObject.HasMember("dir") == false)
			return false;

		const auto& DirValue = ImgObject["dir"];
		if (DirValue.IsArray() == false)
			return false;

		const auto& DirArray = DirValue.GetArray();
		for (const auto& AnimValue : DirArray)
		{
			const std::string_view AnimName = AnimValue["@name"].GetString();
			if (AnimName == InFindAnimName)
				return OutAnimation->ParseBasicAnimationFromWz(AnimValue);
		}

		return false;
	}

private:
	std::unique_ptr<CWzPartsManager> PartsManager;

	std::unordered_map<std::string, CWzCharacterAnimation> WzCharacterAnimations;

	CWzCharacterAnimation* CurrentAnimation = nullptr;

};

