#include "pch.h"
#include "WzSkin.h"

CWzSkinLoader::CWzSkinLoader()
{

}

bool CWzSkinLoader::LoadSkinBaseData(const JValue& InValue, const std::string_view InBaseName)
{
	TWzSkinBaseData* OutSkinBaseData = nullptr;
	if (InBaseName == "front")
		OutSkinBaseData = &Front;
	else if (InBaseName == "back")
		OutSkinBaseData = &Back;
	else
		assert(0);

	const auto& BaseObject = InValue.GetObject();
	for (const auto& Member : BaseObject)
	{
		const std::string_view PartName = Member.name.GetString();
		const auto& PngValue = Member.value;
		EWzSkinType SkinType = CWzSkinLoader::GetSkinTypeByName(PartName);

		auto Iter = OutSkinBaseData->SkinPngs.find(SkinType);
		if (Iter == OutSkinBaseData->SkinPngs.end())
			Iter = OutSkinBaseData->SkinPngs.emplace(SkinType, TWzSkinPng{}).first;
		TWzSkinPng& SkinPng = Iter->second;

		// 여기서 이제 파츠 png데이터 파싱
		if (ParseSkinPng(PngValue, &SkinPng) == false)
			return false;
	}
	return true;
}

bool CWzSkinLoader::ParseSkinPng(const JValue& InPngValue, TWzSkinPng* OutSkinPng)
{
	const auto& MapObject = InPngValue["map"].GetObject();
	for (const auto& Member : MapObject)
	{
		const std::string_view Name = Member.name.GetString();
		if (Member.value.IsString())
		{
			const std::string_view Value = Member.value.GetString();

			Vector2* OutputData = nullptr;
			if (Name == "neck")
				OutputData = &OutSkinPng->Map.Neck;
			else if (Name == "earOverHead")
				OutputData = &OutSkinPng->Map.EarOverHead;
			else if (Name == "earBelowHead")
				OutputData = &OutSkinPng->Map.EarBelowHead;
			else if (Name == "brow")
				OutputData = &OutSkinPng->Map.Brow;
			if (StrToVec2(Value, OutputData) == false)
				return false;
		}
	}
	if (StrToVec2(InPngValue["origin"].GetString(), &OutSkinPng->Origin) == false)
		return false;
	OutSkinPng->Z = InPngValue["z"].GetString();
	OutSkinPng->Group = InPngValue["group"].GetString();

	std::string OutLink = InPngValue["_outlink"].GetString();
	OutLink = "resources/image/" + OutLink + ".png";
	OutSkinPng->OutLink = std::wstring(OutLink.begin(), OutLink.end());

	return true;
}