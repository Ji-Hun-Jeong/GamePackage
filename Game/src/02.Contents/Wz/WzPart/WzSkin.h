#pragma once
#include "../WzBase.h"
#include "Utils.h"
#include "01.Base/Manager/Path.h"

enum class EWzSkinType : uint8_t
{
	Head,
	HumanEar,
	Ear,
	LefEar,
	HighLefEar,
	End,
};

struct TWzSkinMap : public TWzMap
{
	Vector2 EarOverHead;
	Vector2 EarBelowHead;
	Vector2 Brow;
};

struct TWzSkinPng : public TWzPng
{
	TWzSkinMap Map;
	std::string Z;
	std::string Group;
};

struct TWzSkinBaseData
{
	std::map<EWzSkinType, TWzSkinPng> SkinPngs;
};

struct TWzSkinFrameData
{
	TWzSkinPng* RefSkins[static_cast<uint8_t>(EWzSkinType::End)];
};

struct TWzSkinAnimation
{
	std::vector<TWzSkinFrameData> SkinFrames;
};

class CWzSkinLoader
{
	SINGLE(CWzSkinLoader)
public:
	~CWzSkinLoader() = default;

public:
	bool LoadSkinBaseData(const JValue& InValue, const std::string_view InBaseName);

	const TWzSkinAnimation* LoadSkinAnimation(const JValue& InAnimValue, const std::string_view InBaseName)
	{
		if (InAnimValue.IsObject() == false)
			return nullptr;

		auto Iter = SkinAnimations.find(InBaseName.data());
		if (Iter == SkinAnimations.end())
			Iter = SkinAnimations.emplace(InBaseName, TWzSkinAnimation{}).first;
		// 이미 있을때는?

		TWzSkinAnimation& SkinAnimation = Iter->second;

		const auto& FrameObject = InAnimValue.GetObject();
		SkinAnimation.SkinFrames.reserve(FrameObject.MemberCount());
		for (const auto& Frame : FrameObject)
		{
			TWzSkinFrameData& SkinFrameData = SkinAnimation.SkinFrames.emplace_back(TWzSkinFrameData{});

			const std::string_view FrameNumber = Frame.name.GetString();
			const auto& FrameValue = Frame.value;
			const auto& PartObject = FrameValue.GetObject();
			for (const auto& Part : PartObject)
			{
				const std::string_view PartName = Part.name.GetString();
				const std::string_view PartUol = Part.value.GetString();

				EWzSkinType SkinType = CWzSkinLoader::GetSkinTypeByName(PartName);
				std::vector<std::string> Tokens =CPath::RelativePathToTokens(PartUol);

				TWzSkinBaseData* BaseData = nullptr;
				if (Tokens[0] == "front")
					BaseData = &Front;
				else if (Tokens[0] == "back")
					BaseData = &Back;
				else
					assert(0);
				SkinFrameData.RefSkins[static_cast<uint8_t>(SkinType)] = &BaseData->SkinPngs[SkinType];
			}
		}

		return &SkinAnimation;
	}

	static EWzSkinType GetSkinTypeByName(const std::string_view InSkinName)
	{
		if (InSkinName == "head") return EWzSkinType::Head;
		if (InSkinName == "humanEar") return EWzSkinType::HumanEar;
		if (InSkinName == "ear") return EWzSkinType::Ear;
		if (InSkinName == "lefEar") return EWzSkinType::LefEar;
		if (InSkinName == "highlefEar") return EWzSkinType::HighLefEar;
		assert(0);
		return EWzSkinType::Head;
	}

private:
	bool ParseSkinPng(const JValue& InPngValue, TWzSkinPng* OutSkinPng);

private:
	TWzSkinBaseData Front;
	TWzSkinBaseData Back;

	std::map<std::string, TWzSkinAnimation> SkinAnimations;

};