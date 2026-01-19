#pragma once
#include "01.Base/Actor/StaticActor.h"

enum class ESkillType
{
	Active,
	Passive,
	KeyDown,
};

struct TSkillData
{
	ESkillType SkillType = ESkillType::Active;
	std::string Name;
	std::string Description;
};

extern void Serialize(const TSkillData& InSkillData, rapidjson::Value& OutValue, rapidjson::Document::AllocatorType& InAllocator);
extern void DeSerialize(const rapidjson::Value& InValue, TSkillData* OutSkillData);
