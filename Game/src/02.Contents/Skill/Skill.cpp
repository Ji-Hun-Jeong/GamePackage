#include "pch.h"
#include "Skill.h"

#include "GameCore.h"
#include "Utils.h"

CSkillNode* ParseWzData(const JValue& InValue)
{
	CSkillNode* NewNode = new CSkillNode();

	if (InValue.IsObject())
	{
		const auto& Object = InValue.GetObject();
		for (const auto& Member : Object)
		{
			const std::string_view Name = Member.name.GetString();

			CSkillNode* ChildNode = ParseWzData(Member.value);
			if (ChildNode)
				NewNode->AddMember(Name, *ChildNode);
		}
	}
	else if (InValue.IsArray())
	{
		// 배열인 경우 (예: [ {..}, {..} ]) 
		// 키를 "0", "1", "2" 문자열로 변환하여 자식으로 편입
		const auto& Array = InValue.GetArray();
		for (rapidjson::SizeType i = 0; i < Array.Size(); ++i)
		{
			CSkillNode* ChildNode = ParseWzData(Array[i]);
			if (ChildNode)
				NewNode->AddMember(std::to_string(i), *ChildNode);
		}
	}
	else
	{
		if (InValue.IsString())
			NewNode->SetValue(InValue.GetString());
	}

	return NewNode;
}

bool ParseSkillPng(const JValue& InPngValue, TSkillPng* OutSkillPng)
{
	if (InPngValue.IsObject() == false)
		return false;

	// 1. 멤버별 처리를 담당할 람다 정의
	auto ParseMember = [](const std::string_view InName, const JValue& InValue, TSkillPng* OutSkillPng) -> bool
		{
			if (InValue.IsString() == false)
				return false;

			const std::string_view ValStr = InValue.GetString();
			if (InName == "origin")
			{
				if (StrToVec2(ValStr, &OutSkillPng->Origin) == false)
					return false;
			}
			else if (InName == "z")
				OutSkillPng->Z = std::stoi(ValStr.data());
			else if (InName == "_outlink")
				OutSkillPng->OutLink = ValStr;
			else if (InName == "delay")
				OutSkillPng->Delay = std::stoi(ValStr.data());
			else
			{
				CSkillNode* NewNode = ParseWzData(InValue);
				OutSkillPng->AddMember(InName, *NewNode);
			}
			return true;
		};

	const auto& PngObject = InPngValue.GetObject();
	for (const auto& Member : PngObject)
		if (ParseMember(Member.name.GetString(), Member.value, OutSkillPng) == false)
			return false;

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

		for (auto it = Obj.MemberBegin(); it != Obj.MemberEnd(); ++it)
		{
			std::string_view Property = it->name.GetString();
			const auto& Value = it->value;

			// 문자열 데이터들
			if (Property == "mpCon")
				OutSkillData->Common.MpCon = Value.GetString();
			else if (Property == "damage")
				OutSkillData->Common.Damage = Value.GetString();
			else if (Property == "mobCount")
				OutSkillData->Common.MobCount = Value.GetString();
			else if (Property == "attackCount")
				OutSkillData->Common.AttackCount = Value.GetString();
			else if (Property == "maxLevel")
				OutSkillData->Common.MaxLavel = std::stoi(Value.GetString());
			else if (Property == "lt")
				StrToVec2(Value.GetString(), &OutSkillData->Common.LeftTop);
			else if (Property == "rb")
				StrToVec2(Value.GetString(), &OutSkillData->Common.RightBottom);
			else
			{
				CSkillNode* NewNode = ParseWzData(Value);
				OutSkillData->Common.AddMember(Property, *NewNode);
			}
		}
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
				if (fit->value.IsObject())
				{
					TSkillPng png;
					ParseSkillPng(fit->value, &png);
					Hit.Anim.emplace_back(std::move(png));
				}
				else
					Hit.AddMember(fit->name.GetString(), *ParseWzData(fit->value));
			}
		}
	}
	// 5. 기타 정보 (Info)
	else if (InMemberName == "info")
	{
		const auto& Obj = InValue.GetObject();
		if (Obj.HasMember("type"))
			OutSkillData->Info.Type = std::stoi(Obj["type"].GetString());
		if (Obj.HasMember("areaAttack"))
			OutSkillData->Info.AreaAttack = std::stoi(Obj["areaAttack"].GetString());
	}
	else
	{
		CSkillNode* NewNode = ParseWzData(InValue);
		OutSkillData->AddMember(InMemberName, *NewNode);
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
