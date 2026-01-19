#include "pch.h"
#include "Skill.h"

#include "GameCore.h"

void Serialize(const TSkillData& InSkillData, rapidjson::Value& OutValue, rapidjson::Document::AllocatorType& InAllocator)
{
    OutValue.SetObject();

    // Enum을 정수나 문자열로 저장
    OutValue.AddMember("SkillType", static_cast<int>(InSkillData.SkillType), InAllocator);

    // 문자열 저장
    rapidjson::Value name;
    name.SetString(InSkillData.Name.c_str(), InAllocator);
    OutValue.AddMember("Name", name, InAllocator);

    rapidjson::Value desc;
    desc.SetString(InSkillData.Description.c_str(), InAllocator);
    OutValue.AddMember("Description", desc, InAllocator);
}

void DeSerialize(const rapidjson::Value& InValue, TSkillData* OutSkillData)
{
    assert(OutSkillData);
    if (InValue.HasMember("SkillName") && InValue["SkillName"].IsString())
        OutSkillData->Name = InValue["SkillName"].GetString();

    if (InValue.HasMember("SkillType") && InValue["SkillType"].IsInt())
        OutSkillData->SkillType = static_cast<ESkillType>(InValue["SkillType"].GetInt());

    if (InValue.HasMember("Description") && InValue["Description"].IsString())
        OutSkillData->Description = InValue["Description"].GetString();
}