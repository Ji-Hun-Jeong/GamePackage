#pragma once
#include "WzPartData.h"

struct TWzCharacterAnimation
{
	std::string AnimName;
	std::vector<TWzCharacterFrameData> Frames;
	bool bLoop = false;
};

namespace Wz
{
	extern const TWzCharacterFrameData* GetFrameFromCharacterAnimation(const TWzCharacterAnimation& InCharacterAnimation
	, size_t InFrame);
}

class CWzAnimationLoader
{
	SINGLE(CWzAnimationLoader)
public:
	~CWzAnimationLoader() = default;

public:
	TWzCharacterAnimation* ParseWzCharacterAnimation(const JValue& InValue, const std::string_view InAnimName);
	TWzCharacterAnimation* FindCharacterAnimation(const std::string_view InAnimName)
	{
		auto Iter = CharacterAnimations.find(InAnimName);
		if (Iter == CharacterAnimations.end())
			return nullptr;
		return &Iter->second;
	}

private:
	bool BrachPartPng(const JValue& InValue, const std::string_view InName, TWzPartData* OutPartPngData);

	bool ParsePart(const JValue& InValue, TWzPartData* OutPartPngData);

	bool BrachCharacterFrame(const JValue& InValue, const std::string_view InName, TWzCharacterFrameData* OutCharacterFrameData);

	bool ParseCharacterFrame(const JValue& InValue, TWzCharacterFrameData* OutCharacterFrameData);

private:
	std::map<std::string, TWzCharacterAnimation, std::less<>> CharacterAnimations;
	TWzCharacterAnimation* CurrentEditAnimation = nullptr;

};