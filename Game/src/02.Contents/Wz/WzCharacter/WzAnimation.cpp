#include "pch.h"
#include "WzAnimation.h"

#include "01.Base/Manager/Path.h"
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

bool CWzAnimationLoader::ParsePart(const JValue& InValue, TWzPartData* OutPartPngData)
{
	assert(OutPartPngData);
	if (InValue.IsObject() == false)
	{
		if (InValue.IsString() == false)
			return false;
		std::string_view Uol = InValue.GetString();
		std::vector<std::string> Elements = CPath::RelativePathToTokens(Uol);
		size_t RefFrameNumber = 0;
		std::string_view RefPartName;
		TWzCharacterAnimation* RefCharacterAnimation = nullptr;
		if (Elements.size() == 2)
		{
			RefCharacterAnimation = CurrentEditAnimation;
			RefFrameNumber = std::stoi(Elements[0]);
			RefPartName = Elements[1];
		}
		else if (Elements.size() == 3)
		{
			const std::string_view RefAnimName = Elements[0];
			RefCharacterAnimation = FindCharacterAnimation(RefAnimName);
			if (RefCharacterAnimation == nullptr)
				return false;
			RefFrameNumber = std::stoi(Elements[1]);
			RefPartName = Elements[2];
		}
		else
		{
			std::cout << "WzAnimation -> ParseWzCharacterAnimation Exception\n";
			return false;
		}
		EWzPartType RefPartType = Wz::GetPartTypeByName(RefPartName);
		TWzPartData& RefPartData = RefCharacterAnimation->Frames[RefFrameNumber].PartDatas[RefPartType];
		OutPartPngData->Uol = &RefPartData;
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
		return ParsePart(InValue, &PartPngData);
	}
	else if (InName == "action")
		OutCharacterFrameData->Action = InValue.GetString();
	else if (InName == "frame")
		OutCharacterFrameData->Frame = std::stoi(InValue.GetString());
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

	auto& CharacterFrameDatas = CurrentEditAnimation->Frames;
	for (size_t i = 0; i < CharacterFrameDatas.size(); ++i)
	{
		TWzCharacterFrameData& CharacterFrameData = CharacterFrameDatas[i];
		if (CharacterFrameData.Action.empty())
			continue;
		std::string_view AnimName = CharacterFrameData.Action;
		TWzCharacterAnimation* Anim = FindCharacterAnimation(AnimName);
		if (Anim == nullptr)
		{
			std::cout << "CWzAnimationLoader.BrachCharacterFrame: " << AnimName << " Not yet loaded." << std::endl;
			return false;
		}
		int32_t Frame = CharacterFrameData.Frame;
		int32_t Delay = CharacterFrameData.Delay;
		const TWzCharacterFrameData* RefFrameData = Wz::GetFrameFromCharacterAnimation(*Anim
			, static_cast<size_t>(Frame));
		
		OutCharacterFrameData->RefFrame = RefFrameData;
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

	CurrentEditAnimation = &OutCharacterAnimation;

	size_t i = 0;
	for (const auto& Frame : AnimObject)
	{
		const std::string_view FrameNumber = Frame.name.GetString();
		auto& OutFrame = OutCharacterAnimation.Frames[i++];
		if (ParseCharacterFrame(Frame.value, &OutFrame) == false)
		{
			CharacterAnimations.erase(InAnimName.data());
			CurrentEditAnimation = nullptr;
			return nullptr;
		}
	}

	CurrentEditAnimation = nullptr;
	return &OutCharacterAnimation;
}

namespace Wz
{
	const TWzCharacterFrameData* GetFrameFromCharacterAnimation(const TWzCharacterAnimation& InCharacterAnimation
		, size_t InFrame)
	{
		if (InFrame >= InCharacterAnimation.Frames.size())
			return nullptr;
		
		return &InCharacterAnimation.Frames[InFrame];
	}
}

