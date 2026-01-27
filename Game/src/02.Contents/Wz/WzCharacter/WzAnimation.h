#pragma once
#include "../WzPart/WzPartData.h"

namespace Wz
{
	extern const TWzCharacterFrameData* GetFrameFromCharacterAnimation(const TWzAnimation& InCharacterAnimation
	, size_t InFrame);
}

class CWzAnimationLoader
{
	SINGLE(CWzAnimationLoader)
public:
	~CWzAnimationLoader() = default;

public:
	const TWzAnimation* ParseWzCharacterAnimation(const JValue& InValue, const std::string_view InAnimName);
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

	bool BrachFrame(const JValue& InValue, const std::string_view InName, TWzCharacterFrameData* OutCharacterFrameData);

	bool ParseFrame(const JValue& InValue, TWzCharacterFrameData* OutCharacterFrameData);

private:
	std::map<std::string, TWzAnimation, std::less<>> CharacterAnimations;

	TWzAnimation* CurrentEditAnimation = nullptr;

};