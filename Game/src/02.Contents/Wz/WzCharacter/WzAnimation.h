#pragma once
#include "WzPartData.h"

struct TWzAnimation
{
	std::string AnimName;
	std::vector<TWzFrameData> Frames;
	bool bLoop = false;
};

namespace Wz
{
	extern const TWzFrameData* GetFrameFromCharacterAnimation(const TWzAnimation& InCharacterAnimation
	, size_t InFrame);
}

class CWzAnimationLoader
{
	SINGLE(CWzAnimationLoader)
public:
	~CWzAnimationLoader() = default;

public:
	TWzAnimation* ParseWzCharacterAnimation(const JValue& InValue, const std::string_view InAnimName);
	void ParseWzDefaultSkinAnimation(const JValue& InValue, const std::string_view InAnimName);
	TWzAnimation* ParseWzSkinAnimation(const JValue& InValue, const std::string_view InAnimName);
	TWzAnimation* FindCharacterAnimation(const std::string_view InAnimName)
	{
		auto Iter = CharacterAnimations.find(InAnimName);
		if (Iter == CharacterAnimations.end())
			return nullptr;
		return &Iter->second;
	}	

private:
	bool BrachPartPng(const JValue& InValue, const std::string_view InName, TWzPartData* OutPartPngData);

	bool ParsePart(const JValue& InValue, TWzPartData* OutPartPngData);

	bool BrachFrame(const JValue& InValue, const std::string_view InName, TWzFrameData* OutCharacterFrameData);

	bool ParseFrame(const JValue& InValue, TWzFrameData* OutCharacterFrameData);

private:
	std::map<std::string, TWzAnimation, std::less<>> CharacterAnimations;
	std::map<std::string, TWzAnimation, std::less<>> SkinAnimations;
	TWzFrameData FrontSkin;
	TWzFrameData BackSkin;

	TWzAnimation* CurrentEditAnimation = nullptr;

};