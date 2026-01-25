#include "pch.h"
#include "WzPartData.h"

#include "Utils.h"

namespace Wz
{
	bool BrachPartPng(const JValue& InValue, const std::string_view InName, TWzPartData* OutPartPngData)
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

	EWzPartType GetPartTypeByName(const std::string_view InPartName)
	{
		if (InPartName == "body")			return EWzPartType::Body;
		if (InPartName == "arm")				return EWzPartType::Arm;
		if (InPartName == "hand")			return EWzPartType::Hand;
		if (InPartName == "armOverHair")		return EWzPartType::ArmOverHair;
		if (InPartName == "lHand")			return EWzPartType::LHand;
		if (InPartName == "rHand")			return EWzPartType::RHand;
		return EWzPartType::End;
	}

	bool ParsePartPng(const JValue& InValue, TWzPartData* OutPartPngData)
	{
		assert(OutPartPngData);
		if (InValue.IsObject() == false)
			return false;

		const auto& PartObject = InValue.GetObject();
		for (const auto& Member : PartObject)
		{
			const std::string_view Name = Member.name.GetString();
			if (BrachPartPng(Member.value, Name, OutPartPngData) == false)
				return false;
		}
		return true;
	}
	bool BrachCharacterFrame(const JValue& InValue, const std::string_view InName, TWzCharacterFrameData* OutCharacterFrameData)
	{
		assert(OutCharacterFrameData);
		if (InName == "face")
			OutCharacterFrameData->Face = std::stoi(InValue.GetString());
		else if (InName == "delay")
			OutCharacterFrameData->Delay = std::stoi(InValue.GetString());
		else if (InName == "body" || InName == "arm" || InName == "hand" ||
			InName == "armOverHair" || InName == "lHand" || InName == "rHand")
		{
			OutCharacterFrameData->PartDatas.emplace_back();
			auto& PartPngData = OutCharacterFrameData->PartDatas.back();
			EWzPartType PartType = GetPartTypeByName(InName);
			PartPngData.WzPartType = PartType;
			return ParsePartPng(InValue, &PartPngData);
		}
		else
		{
			CWzNode* Node = Wz::GenerateWzData(InValue);
			OutCharacterFrameData->AddMember(InName, *Node);
		}
		return true;
	}

	bool ParseCharacterFrame(const JValue& InValue, TWzCharacterFrameData* OutCharacterFrameData)
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
}

