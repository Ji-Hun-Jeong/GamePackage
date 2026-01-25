#include "pch.h"
#include "WzAnimation.h"

#include "Utils.h"

CWzAnimationLoader::CWzAnimationLoader()
{

}

bool CWzAnimationLoader::BrachPartPng(const JValue& InValue, const std::string_view InName, TWzPartData* OutPartPngData)
{
	if (InName == "origin")
	{
		Vector2 Origin;
		if (StrToVec2(InValue.GetString(), &Origin) == false)
			return false;
		OutPartPngData->Origin = Origin;
	}
	else if (InName == "map")
	{
		const auto& MapObject = InValue.GetObject();
		for (const auto& Member : MapObject)
		{
			const std::string_view Name = Member.name.GetString();
			if (Member.value.IsString())
			{
				const std::string_view Value = Member.value.GetString();

				Vector2* OutputData = nullptr;
				if (Name == "neck")
					OutputData = &OutPartPngData->Map.Neck;
				else if (Name == "navel")
					OutputData = &OutPartPngData->Map.Navel;
				else if (Name == "hand")
					OutputData = &OutPartPngData->Map.Hand;
				else if (Name == "handMove")
					OutputData = &OutPartPngData->Map.HandMove;
				if (StrToVec2(Value, OutputData) == false)
					return false;
			}
			else
			{
				CWzNode* Node = Wz::GenerateWzData(Member.value);
				OutPartPngData->AddMember(InName, *Node);
			}
		}
	}
	else if (InName == "z")
		OutPartPngData->Z = InValue.GetString();
	else if (InName == "group")
		OutPartPngData->Group = InValue.GetString();
	else if (InName == "_outlink")
	{
		std::string Str = InValue.GetString();
		Str = "resources/image/" + Str + ".png";
		std::wstring OutLink = std::wstring(Str.begin(), Str.end());
		OutPartPngData->OutLink = OutLink;
	}
	else
	{
		CWzNode* Node = Wz::GenerateWzData(InValue);
		OutPartPngData->AddMember(InName, *Node);
	}
	return true;
}

bool CWzAnimationLoader::ParsePartPng(const JValue& InValue, TWzPartData* OutPartPngData)
{
	assert(OutPartPngData);
	if (InValue.IsObject() == false)
	{
		if (InValue.IsString() == false)
			return false;
		OutPartPngData->Uol = InValue.GetString();
	}
	else
	{
		const auto& PartObject = InValue.GetObject();
		for (const auto& Member : PartObject)
		{
			const std::string_view Name = Member.name.GetString();
			if (BrachPartPng(Member.value, Name, OutPartPngData) == false)
				return false;
		}
	}
	return true;
}
bool CWzAnimationLoader::BrachCharacterFrame(const JValue& InValue, const std::string_view InName, TWzCharacterFrameData* OutCharacterFrameData)
{
	assert(OutCharacterFrameData);
	if (InName == "face")
		OutCharacterFrameData->Face = std::stoi(InValue.GetString());
	else if (InName == "delay")
		OutCharacterFrameData->Delay = std::stoi(InValue.GetString());
	else if (InName == "body" || InName == "arm" || InName == "hand" ||
		InName == "armOverHair" || InName == "lHand" || InName == "rHand")
	{
		EWzPartType PartType = Wz::GetPartTypeByName(InName);
		auto Pair = OutCharacterFrameData->PartDatas.emplace(PartType, TWzPartData{});
		auto& PartPngData = Pair.first->second;
		return ParsePartPng(InValue, &PartPngData);
	}
	else if (InName == "action")
		OutCharacterFrameData->Action = InValue.GetString();
	else
	{
		CWzNode* Node = Wz::GenerateWzData(InValue);
		OutCharacterFrameData->AddMember(InName, *Node);
	}
	return true;
}

bool CWzAnimationLoader::ParseCharacterFrame(const JValue& InValue, TWzCharacterFrameData* OutCharacterFrameData)
{
	if (InValue.IsObject() == false)
		return false;
	const auto& FrameObject = InValue.GetObject();

	for (const auto& FrameData : FrameObject)
	{
		const std::string_view FrameDataName = FrameData.name.GetString();
		if (BrachCharacterFrame(FrameData.value, FrameDataName, OutCharacterFrameData) == false)
			return false;
	}
	return true;
}

TWzCharacterAnimation* CWzAnimationLoader::ParseWzCharacterAnimation(const JValue& InValue, const std::string_view InAnimName)
{
	if (InValue.IsObject() == false)
		return nullptr;
	const auto& AnimObject = InValue.GetObject();

	auto Pair = CharacterAnimations.emplace(InAnimName, TWzCharacterAnimation{});
	TWzCharacterAnimation& OutCharacterAnimation = Pair.first->second;
	OutCharacterAnimation.Frames.resize(AnimObject.MemberCount());
	OutCharacterAnimation.AnimName = InAnimName;

	size_t i = 0;
	for (const auto& Frame : AnimObject)
	{
		const std::string_view FrameNumber = Frame.name.GetString();
		auto& OutFrame = OutCharacterAnimation.Frames[i++];
		if (ParseCharacterFrame(Frame.value, &OutFrame) == false)
		{
			CharacterAnimations.erase(InAnimName.data());
			return nullptr;
		}
	}

	for (auto& Frame : OutCharacterAnimation.Frames)
	{
		auto& Parts = Frame.PartDatas;
		for (auto& Pair : Parts)
		{
			TWzPartData& PartData = Pair.second;
			if (PartData.Uol.empty())
				continue;

		}
	}

	return &OutCharacterAnimation;
}