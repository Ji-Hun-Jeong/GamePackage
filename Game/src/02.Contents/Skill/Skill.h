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
	ESkillType SkillType;
	std::string Name;
	std::string Description;
};
