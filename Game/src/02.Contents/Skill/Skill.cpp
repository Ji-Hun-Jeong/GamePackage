#include "pch.h"
#include "Skill.h"

#include "GameCore.h"
#include "Utils.h"

bool ParseSkillPng(const JValue& InPngValue, TSkillPng* OutSkillPng)
{
	if (InPngValue.IsObject() == false)
		return false;

	const auto& PngObject = InPngValue.GetObject();
	if (PngObject.HasMember("origin"))
	{
		const auto& OriginValue = PngObject["origin"];
		if (OriginValue.IsString() == false)
			return false;
		if (StrToVec2(OriginValue.GetString(), &OutSkillPng->Origin) == false)
			return false;
	}

	if (PngObject.HasMember("z"))
	{
		const auto& ZValue = PngObject["z"];
		if (ZValue.IsString() == false)
			return false;
		int32_t Z = std::stoi(ZValue.GetString());
		OutSkillPng->Z = Z;
	}

	if (PngObject.HasMember("_outlink"))
	{
		const auto& OutLinkValue = PngObject["_outlink"];
		if (OutLinkValue.IsString() == false)
			return false;
		OutSkillPng->OutLink = OutLinkValue.GetString();
	}

	if (PngObject.HasMember("delay"))
	{
		const auto& DelayValue = PngObject["delay"];
		if (DelayValue.IsString() == false)
			return false;
		OutSkillPng->Delay = std::stoi(DelayValue.GetString());
	}

	return true;
}

bool ParseSkillData(const std::string_view InMemberName, const JValue& InValue, TSkillData* OutSkillData)
{
    // 1. 단순 값 처리 (int 캐스팅 필요)
    if (InMemberName == "weapon")
        OutSkillData->Weapon = std::stoi(InValue.GetString());

    else if (InMemberName == "subWeapon")
        OutSkillData->SubWeapon = std::stoi(InValue.GetString());

    else if (InMemberName == "icon")
        ParseSkillPng(InValue, &OutSkillData->Icon);

    else if (InMemberName == "iconMouseOver")
        ParseSkillPng(InValue, &OutSkillData->IconMouseOver);

    else if (InMemberName == "iconDisabled") 
        ParseSkillPng(InValue, &OutSkillData->IconDisabled);

    // 3. 복합 객체 처리 (Common)
    else if (InMemberName == "common") 
    {
        const auto& Obj = InValue.GetObject();
        if (Obj.HasMember("mpCon")) OutSkillData->Common.MpCon = Obj["mpCon"].GetString();
        if (Obj.HasMember("damage")) OutSkillData->Common.Damage = Obj["damage"].GetString();
        if (Obj.HasMember("mobCount")) OutSkillData->Common.MobCount = Obj["mobCount"].GetString();
        if (Obj.HasMember("attackCount")) OutSkillData->Common.AttackCount = Obj["attackCount"].GetString();
        if (Obj.HasMember("maxLevel")) OutSkillData->Common.MaxLavel = std::stoi(Obj["maxLevel"].GetString());
        if (Obj.HasMember("lt")) StrToVec2(Obj["lt"].GetString(), &OutSkillData->Common.LeftTop);
        if (Obj.HasMember("rb")) StrToVec2(Obj["rb"].GetString(), &OutSkillData->Common.RightBottom);
    }

    else if (InMemberName == "effect") 
    {
        const auto& Obj = InValue.GetObject();
        OutSkillData->Effect.Anim.resize(Obj.MemberCount());
        size_t i = 0;
        for (auto it = Obj.MemberBegin(); it != Obj.MemberEnd(); ++it) 
            ParseSkillPng(it->value, &OutSkillData->Effect.Anim[i++]);
    }
    else if (InMemberName == "hit") 
    {
        const auto& Obj = InValue.GetObject();
        OutSkillData->Hit.resize(Obj.MemberCount());
        size_t i = 0;
        for (auto it = Obj.MemberBegin(); it != Obj.MemberEnd(); ++it) 
        {
            auto& Hit = OutSkillData->Hit[i++];
            const auto& Frames = it->value.GetObject();
            Hit.Anim.reserve(Frames.MemberCount());
            for (auto fit = Frames.MemberBegin(); fit != Frames.MemberEnd(); ++fit) 
            {
                if (!fit->value.IsObject()) continue;
                TSkillPng png;
                ParseSkillPng(fit->value, &png);
                Hit.Anim.emplace_back(std::move(png));
            }
        }
    }
    // 5. 기타 정보 (Info)
    else if (InMemberName == "info") 
    {
        const auto& Obj = InValue.GetObject();
        if (Obj.HasMember("type")) OutSkillData->Info.Type = std::stoi(Obj["type"].GetString());
        if (Obj.HasMember("areaAttack")) OutSkillData->Info.AreaAttack = std::stoi(Obj["areaAttack"].GetString());
    }
    else 
    {
        const auto& Object = InValue.GetObject();
        // 이거 할지 말지 결정좀하자
        return false;
    }

    return true;
}

bool DeSerializeSkillData(const rapidjson::Value& InValue, const std::string_view InSkillCode, TSkillData* OutSkillData)
{
    assert(OutSkillData);

    if (!InValue.HasMember(InSkillCode.data()))
        return false;

    const auto& SkillValue = InValue[InSkillCode.data()];
    if (!SkillValue.IsObject()) return false;

    // 루프를 돌며 멤버별로 분기 함수 호출
    for (auto it = SkillValue.MemberBegin(); it != SkillValue.MemberEnd(); ++it)
    {
        std::string_view memberName = it->name.GetString();
        ParseSkillData(memberName, it->value, OutSkillData);
    }

    return true;
}